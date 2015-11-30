//
//  Game.h
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef GAME_H
#define GAME_H

#include "Constants.h"
#include "OpenGL.h"
#include "Box2D.h"
#include <vector>


#define BOX2D_DRAW_DEBUG_DATA 1

class GameObject;

class Game : public b2ContactListener
{
public:
    //Singleton instance methods
    static Game* getInstance();
    
    //Update, paint and touch event (lifecycle) methods
    void update(double delta);
    void paint();
    void touchEvent(TouchEvent touchEvent, float locationX, float locationY, float previousX, float previousY);
    
    //Reset methods
    void reset();
    
    //Conveniance methods to access the screen's width and height
    float getScreenWidth();
    float getScreenHeight();
    float getScale();

    //Loading methods
    bool isLoading();
    
    //Box2D helper methods
    b2Body* createPhysicsBody(const b2BodyDef* bodyDef, const b2FixtureDef* fixtureDef = NULL);
    void destroyPhysicsBody(b2Body* body);
    
    b2Joint* createJoint(const b2JointDef* jointDef);
    void destroyJoint(b2Joint* joint);
    
    b2Body* createBox(b2Vec2 position, b2Vec2 size, b2BodyType bodyType, unsigned short category = 0x0001, unsigned short mask = 0xFFFF);
    b2Body* createCircle(b2Vec2 position, float radius, b2BodyType bodyType, unsigned short category = 0x0001, unsigned short mask = 0xFFFF);
    
    //b2ContactListener methods
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);

private:
    //Private constructor and destructor ensures the singleton instance
    Game();
    ~Game();
    
    //Load method, called once every load step
    void load();
    void paintLoading();
    
    //Singleton instance static member variable
    static Game* m_Instance;
    
    //Load step member variable
    int m_LoadStep;
    
    //Box2D members
    b2World* m_World;
    b2DebugDraw* m_DebugDraw;
    
    //Game objects vector
    std::vector<GameObject*> m_GameObjects;
};

#endif
