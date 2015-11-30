//
//  Game.cpp
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "Game.h"
#include "GameObject.h"
#include "DeviceUtils.h"
#include "MathUtils.h"
#include "PhysicsEditorWrapper.h"


Game* Game::m_Instance = NULL;

Game* Game::getInstance()
{
    //Singleton design pattern ensures that there is only 1 instance of the game
    if(m_Instance == NULL)
    {
        m_Instance = new Game();
    }
    return m_Instance;
}

Game::Game() :
    m_LoadStep(0),
    m_World(NULL),
    m_DebugDraw(NULL)
{
    
}

Game::~Game()
{
    //Delete the game objects
    for(int i = 0; i < m_GameObjects.size(); i++)
    {
        delete m_GameObjects.at(i);
        m_GameObjects.at(i) = NULL;
    }
    m_GameObjects.clear();

    //Delete the debug draw instance
    if(m_DebugDraw != NULL)
    {
        delete m_DebugDraw;
        m_DebugDraw = NULL;
    }
    
    //Delete the Box2D world instance, MAKES SURE this is the last object deleted
    if(m_World != NULL)
    {
        //Destroy all the bodies in the world
        b2Body* body = m_World->GetBodyList();
        while(body != NULL)
        {
            //Destroy the body and get the next body (if there is one)
            b2Body* nextBody = body->GetNext();
            destroyPhysicsBody(body);
            body = nextBody;
        }
        
        //Finally delete the world
        delete m_World;
        m_World = NULL;
    }
}

void Game::load()
{
    switch(m_LoadStep)
    {
        case GameLoadStepInitial:
        {
            //TODO: Load game content required for future load steps here
        }
        break;
            
        case GameLoadStepWorld:
        {
            //Define the gravity vector.
            b2Vec2 gravity;
            gravity.Set(GAME_GRAVITY_X, GAME_GRAVITY_Y);
            
            //Construct the Box2d world object, which will
            //holds and simulates the rigid bodies
            m_World = new b2World(gravity);
            m_World->SetContinuousPhysics(GAME_PHYSICS_CONTINUOUS_SIMULATION);
            m_World->SetContactListener(this);
            
            #if DEBUG
            //Create the debug draw for Box2d
            m_DebugDraw = new b2DebugDraw(b2Helper::box2dRatio());
            
            //Set the debug draw flags
            uint32 flags = 0;
            flags += b2Draw::e_shapeBit;
            flags += b2Draw::e_jointBit;
            flags += b2Draw::e_centerOfMassBit;
            m_DebugDraw->SetFlags(flags);
            
            //Set the Box2d world debug draw instance
            m_World->SetDebugDraw(m_DebugDraw);
            #endif

            //Define the ground body.
            b2BodyDef groundBodyDef;
            groundBodyDef.position.Set(0.0f, 0.0f); // bottom-left corner
            
            //Call the body factory which allocates memory for the ground body
            //from a pool and creates the ground box shape (also from a pool).
            //The body is also added to the world.
            b2Body* groundBody = createPhysicsBody(&groundBodyDef);
            
            //Convert the screen width and height to meters
            float width = b2Helper::screenSpaceToBox2dSpace(DeviceUtils::getScreenResolutionWidth());

            //Define the ground box shape.
            b2EdgeShape groundShape;
            groundShape.Set(b2Vec2(0.0f, 0.0f), b2Vec2(width, 0.0f));
            groundBody->CreateFixture(&groundShape, 0.0f);
            
            //Load the box2d shape definitions
            PhysicsEditorCpp::addShapesFromPlist("shapedefs.plist");
        }
        break;
            
            
            //TODO: Load additional game content here, add additional steps in the GameConstants.h enum
            
            
        case GameLoadStepFinal:
        {
            reset();
        }
        break;
            
        default:
            break;
    }
    
    //Increment the load step
    m_LoadStep++;
}

void Game::update(double aDelta)
{
    //While the game is loading, the load method will be called once per update
    if(isLoading() == true)
    {
        load();
        return;
    }
    
    //Step the Box2D world this update cycle
    if(m_World != NULL)
    {
        m_World->Step(aDelta, GAME_PHYSICS_VELOCITY_ITERATIONS, GAME_PHYSICS_POSITION_ITERATIONS);
    }
    
    //Update the game's game objects
    for(int i = 0; i < m_GameObjects.size(); i++)
    {
        m_GameObjects.at(i)->update(aDelta);
    }
}

void Game::paint()
{
    //While the game is loading, the load method will be called once per update
    if(isLoading() == true)
    {
        paintLoading();
        return;
    }
    
    //Paint the game's game objects
    for(int i = 0; i < m_GameObjects.size(); i++)
    {
        m_GameObjects.at(i)->paint();
    }
    
#if DEBUG && BOX2D_DRAW_DEBUG_DATA
    if(m_World != NULL)
    {
        m_World->DrawDebugData();
    }
#endif
}

void Game::paintLoading()
{
    //Cache the screen width and height
    float screenWidth = getScreenWidth();
    float screenHeight = getScreenHeight();

    //Draw a black background, you could replace this
    //in the future with a full screen background texture
    OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorBlack());
    OpenGLRenderer::getInstance()->drawRectangle(0.0f, 0.0f, screenWidth, screenHeight);
    
    //Calculate the bar width and height, don't actually hard-code these
    float barWidth = 200.0f * getScale();
    float barHeight = 40.0f * getScale();
    float barX = (screenWidth - barWidth) / 2.0f;
    float barY = (screenHeight - barHeight) / 2.0f;
    
    float percentageLoaded = (float)m_LoadStep / (float)(GameLoadStepCount - 1);
    float loadedWidth = barWidth * percentageLoaded;
    OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorYellow());
    OpenGLRenderer::getInstance()->drawRectangle(barX, barY, loadedWidth, barHeight);
    
    OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorWhite());
    OpenGLRenderer::getInstance()->drawRectangle(barX, barY, barWidth, barHeight, false);
}

void Game::touchEvent(TouchEvent touchEvent, float locationX, float locationY, float previousX, float previousY)
{

}

void Game::reset()
{
    //Reset the game's game objects
    for(int i = 0; i < m_GameObjects.size(); i++)
    {
        m_GameObjects.at(i)->reset();
    }
}

b2Body* Game::createPhysicsBody(const b2BodyDef* bodyDef, const b2FixtureDef* fixtureDef)
{
    if(bodyDef != NULL)
    {
        b2Body* body = m_World->CreateBody(bodyDef);
        
        if(fixtureDef != NULL)
        {
            body->CreateFixture(fixtureDef);
        }
        
        return body;
    }
    return NULL;
}

void Game::destroyPhysicsBody(b2Body* body)
{
    //Safety check that aBody isn't NULL
    if(body != NULL)
    {
        //Destroy all the fixtures attached to the body
        b2Fixture* fixture = body->GetFixtureList();
        while(fixture != NULL)
        {
            b2Fixture* nextFixture = fixture->GetNext();
            body->DestroyFixture(fixture);
            fixture = nextFixture;
        }
        
        //Destroy the body
        m_World->DestroyBody(body);
    }
}

b2Joint* Game::createJoint(const b2JointDef* jointDef)
{
    if(jointDef != NULL)
    {
        return m_World->CreateJoint(jointDef);
    }
    return NULL;
}

void Game::destroyJoint(b2Joint* joint)
{
    if(joint != NULL)
    {
        m_World->DestroyJoint(joint);
    }
}

b2Body* Game::createBox(b2Vec2 position, b2Vec2 size, b2BodyType bodyType, unsigned short category, unsigned short mask)
{
    b2BodyDef bodyDef;
    bodyDef.type = bodyType;
    bodyDef.position = position;
    bodyDef.angle = 0.0f;
    
    b2PolygonShape boxShape;
    boxShape.SetAsBox(size.x, size.y);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0f;
    fixtureDef.filter.categoryBits = category;
    fixtureDef.filter.maskBits = mask;
    
    b2Body* body = createPhysicsBody(&bodyDef, &fixtureDef);
    return body;
}

b2Body* Game::createCircle(b2Vec2 position, float radius, b2BodyType bodyType, unsigned short category, unsigned short mask)
{
    b2BodyDef bodyDef;
    bodyDef.type = bodyType;
    bodyDef.position = position;
    bodyDef.angle = 0.0f;
    
    b2CircleShape circleShape;
    circleShape.m_radius = radius;
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;
    fixtureDef.filter.categoryBits = category;
    fixtureDef.filter.maskBits = mask;
    
    b2Body* body = createPhysicsBody(&bodyDef, &fixtureDef);
    return body;
}

void Game::BeginContact(b2Contact* contact)
{
    //Handle contacts here
}

void Game::EndContact(b2Contact* contact)
{
    //Handle contacts here
}

float Game::getScreenWidth()
{
    return DeviceUtils::getScreenResolutionWidth();
}

float Game::getScreenHeight()
{
    return DeviceUtils::getScreenResolutionHeight();
}

float Game::getScale()
{
    return DeviceUtils::getContentScaleFactor();
}

bool Game::isLoading()
{
    return m_LoadStep < GameLoadStepCount;
}
