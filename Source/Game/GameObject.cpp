//
//  GameObject.cpp
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "GameObject.h"
#include "MathUtils.h"


GameObject::GameObject() :
    m_Texture(NULL),
    m_Body(NULL),
    m_X(0.0f),
    m_Y(0.0f),
    m_UpdateTransform(false),
    m_IsActive(false),
    m_UpdateActive(false),
    m_LinearVelocity(b2Vec2(0.0f, 0.0f)),
    m_UpdateLinearVelocity(false),
    m_AngularVelocity(0.0f),
    m_UpdateAngularVelocity(false),
    m_Force(b2Vec2(0.0f, 0.0f)),
    m_ForcePoint(b2Vec2(0.0f, 0.0f)),
    m_UpdateForce(false),
    m_Torque(0.0f),
    m_UpdateTorque(false),
    m_LinearImpulse(b2Vec2(0.0f, 0.0f)),
    m_LinearImpulsePoint(b2Vec2(0.0f, 0.0f)),
    m_UpdateLinearImpulse(false),
    m_AngularImpulse(0.0f),
    m_UpdateAngularImpulse(false),
    m_GravityScale(1.0f),
    m_UpdateGravityScale(false),
    m_BodyType(b2_staticBody),
    m_UpdateBodyType(false)
{

}

GameObject::GameObject(b2Body* body) :
    m_Texture(NULL),
    m_Body(body),
    m_X(0.0f),
    m_Y(0.0f),
    m_UpdateTransform(false),
    m_IsActive(m_Body->IsActive()),
    m_UpdateActive(false),
    m_LinearVelocity(b2Vec2(0.0f, 0.0f)),
    m_UpdateLinearVelocity(false),
    m_AngularVelocity(0.0f),
    m_UpdateAngularVelocity(false),
    m_Force(b2Vec2(0.0f, 0.0f)),
    m_ForcePoint(b2Vec2(0.0f, 0.0f)),
    m_UpdateForce(false),
    m_Torque(0.0f),
    m_UpdateTorque(false),
    m_LinearImpulse(b2Vec2(0.0f, 0.0f)),
    m_LinearImpulsePoint(b2Vec2(0.0f, 0.0f)),
    m_UpdateLinearImpulse(false),
    m_AngularImpulse(0.0f),
    m_UpdateAngularImpulse(false),
    m_GravityScale(1.0f),
    m_UpdateGravityScale(false),
    m_BodyType(m_Body->GetType()),
    m_UpdateBodyType(false)
{
    //Set the body's 'user data'
    m_Body->SetUserData(this);
}

GameObject::~GameObject()
{
    if(m_Texture != NULL)
    {
        delete m_Texture;
        m_Texture = NULL;
    }
}

void GameObject::update(double aDelta)
{
    if(m_Texture != NULL)
    {
        if(m_Texture->isAnimated() == true)
        {
            OpenGLAnimatedTexture* animatedTexture = (OpenGLAnimatedTexture*)m_Texture;
            animatedTexture->update(aDelta);
        }
    }
    
    //Safety check that the body
    if(m_Body == NULL)
    {
        return;
    }
    
    if(m_UpdateTransform == true)
    {
        m_UpdateTransform = false;
        b2Vec2 position = b2Helper::screenSpaceToBox2dSpace(m_X, m_Y);
        float angle = MathUtils::degressToRadians(m_Angle);
        m_Body->SetTransform(position, angle);
    }
    
    if(m_UpdateActive == true)
    {
        m_UpdateActive = false;
        m_Body->SetActive(m_IsActive);
    }
    
    if(m_UpdateBodyType == true)
    {
        m_UpdateBodyType = false;
        m_Body->SetType(m_BodyType);
    }
    
    if(m_UpdateLinearVelocity == true)
    {
        m_UpdateLinearVelocity = false;
        m_Body->SetLinearVelocity(m_LinearVelocity);
    }
    
    if(m_UpdateAngularVelocity == true)
    {
        m_UpdateAngularVelocity = false;
        m_Body->SetAngularVelocity(m_AngularVelocity);
    }
    
    if(m_UpdateForce == true)
    {
        m_UpdateForce = false;
        m_Body->ApplyForce(m_Force, m_ForcePoint);
    }
    
    if(m_UpdateTorque == true)
    {
        m_UpdateTorque = false;
        m_Body->ApplyTorque(m_Torque);
    }
    
    if(m_UpdateLinearImpulse == true)
    {
        m_UpdateLinearImpulse = false;
        m_Body->ApplyLinearImpulse(m_LinearImpulse, m_LinearImpulsePoint);
    }
    
    if(m_UpdateAngularImpulse == true)
    {
        m_UpdateAngularImpulse = false;
        m_Body->ApplyAngularImpulse(m_AngularImpulse);
    }
    
    if(m_UpdateGravityScale == true)
    {
        m_UpdateGravityScale = false;
        m_Body->SetGravityScale(m_GravityScale);
    }
}

void GameObject::paint()
{
    if(m_Texture != NULL)
    {
        OpenGLRenderer::getInstance()->drawTexture(m_Texture, getX(), getY(), getAngle());
    }
}

void GameObject::reset()
{
    setPosition(0.0f, 0.0f);
}

void GameObject::setX(float x)
{
    m_X = x;
    m_UpdateTransform = true;
}

void GameObject::setY(float y)
{
    m_Y = y;
    m_UpdateTransform = true;
}

void GameObject::setPosition(float x, float y)
{
    setX(x);
    setY(y);
}

float GameObject::getX()
{
    if(m_Body != NULL)
    {
        return b2Helper::box2dSpaceToScreenSpace(m_Body->GetPosition().x);
    }
    return 0.0f;
}

float GameObject::getY()
{
    if(m_Body != NULL)
    {
        return b2Helper::box2dSpaceToScreenSpace(m_Body->GetPosition().y);
    }
    return 0.0f;
}

void GameObject::getPosition(float& x, float& y)
{
    x = getX();
    y = getY();
}

float GameObject::getAngle()
{
    if(m_Body != NULL)
    {
        return MathUtils::radiansToDegrees(m_Body->GetAngle());
    }
    return 0.0f;
}

void GameObject::setAngle(float angle)
{
    m_Angle = angle;
    m_UpdateTransform = true;
}

bool GameObject::isActive()
{
    if(m_Body != NULL)
    {
        return m_Body->IsActive();
    }
    return false;
}

void GameObject::setIsActive(bool active)
{
    m_IsActive = active;
    m_UpdateActive = true;
}

void GameObject::setLinearVelocity(const b2Vec2 velocity)
{
    m_LinearVelocity = velocity;
    m_UpdateLinearVelocity = true;
}

b2Vec2 GameObject::getLinearVelocity()
{
    if(m_Body != NULL)
    {
        return m_Body->GetLinearVelocity();
    }
    return b2Vec2(0.0f, 0.0f);
}

void GameObject::setAngularVelocity(float omega)
{
    m_AngularVelocity = omega;
    m_UpdateAngularVelocity = true;
}

float GameObject::getAngularVelocity()
{
    if(m_Body != NULL)
    {
        return m_Body->GetAngularVelocity();
    }
    return 0.0f;
}

void GameObject::applyForce(const b2Vec2 force, const b2Vec2 point)
{
    m_Force = force;
    m_ForcePoint = point;
    m_UpdateForce = true;
}

void GameObject::applyTorque(float torque)
{
    m_Torque = torque;
    m_UpdateTorque = true;
}

void GameObject::applyLinearImpulse(const b2Vec2 impulse, const b2Vec2 point)
{
    m_LinearImpulse = impulse;
    m_LinearImpulsePoint = point;
    m_UpdateLinearImpulse = true;
}

void GameObject::applyAngularImpulse(float impulse)
{
    m_AngularImpulse = impulse;
    m_UpdateAngularImpulse = true;
}

void GameObject::setGravityScale(float gravityScale)
{
    m_GravityScale = gravityScale;
    m_UpdateGravityScale = true;
}

float GameObject::getGravityScale()
{
    if(m_Body != NULL)
    {
        return m_Body->GetGravityScale();
    }
    return 0.0f;
}

void GameObject::setBodyType(b2BodyType bodyType)
{
    m_BodyType = bodyType;
    m_UpdateBodyType = true;
}

b2BodyType GameObject::getBodyType()
{
    if(m_Body != NULL)
    {
        return m_Body->GetType();
    }
    return b2_staticBody;
}

bool GameObject::testPoint(float x, float y)
{
    bool containsPoint = false;
    
    if(m_Body != NULL)
    {
        b2Fixture* fixture = m_Body->GetFixtureList();
        b2Vec2 position = b2Helper::screenSpaceToBox2dSpace(x, y);
        
        while (fixture != NULL && containsPoint == false)
        {
            containsPoint = fixture->TestPoint(position);
            fixture = fixture->GetNext();
        }
    }
    
    return containsPoint;
}

b2Body* GameObject::getBody()
{
    return m_Body;
}
