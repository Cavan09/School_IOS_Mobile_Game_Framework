//
//  GameObject.h
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Game.h"
#include "OpenGL.h"
#include "Box2D.h"


class OpenGLTexture;

class GameObject
{
public:
    GameObject();
    GameObject(b2Body* body);
    virtual ~GameObject();
    
    virtual const char* getType() = 0;
    
    virtual void update(double delta);
    virtual void paint();
    
    virtual void reset();
    
    virtual void setX(float x);
    virtual void setY(float y);
    virtual void setPosition(float x, float y);
    
    virtual float getX();
    virtual float getY();
    virtual void getPosition(float& x, float& y);
    
    virtual float getAngle();
    virtual void setAngle(float angle);
    
    virtual bool isActive();
    virtual void setIsActive(bool active);
    
    virtual void setLinearVelocity(const b2Vec2 velocity);
	virtual b2Vec2 getLinearVelocity();
    
	virtual void setAngularVelocity(float omega);
	virtual float getAngularVelocity();

	virtual void applyForce(const b2Vec2 force, const b2Vec2 point);
	virtual void applyTorque(float torque);

	virtual void applyLinearImpulse(const b2Vec2 impulse, const b2Vec2 point);
	virtual void applyAngularImpulse(float impulse);
    
    virtual void setGravityScale(float gravityScale);
    virtual float getGravityScale();
    
    virtual void setBodyType(b2BodyType bodyType);
    virtual b2BodyType getBodyType();
    
    virtual bool testPoint(float x, float y);
    
    virtual b2Body* getBody();
    
protected:
    OpenGLTexture* m_Texture;
    b2Body* m_Body;
    
    float m_X;
    float m_Y;
    float m_Angle;
    bool m_UpdateTransform;
    
    bool m_IsActive;
    bool m_UpdateActive;
    
    b2Vec2 m_LinearVelocity;
    bool m_UpdateLinearVelocity;
    
    float m_AngularVelocity;
    bool m_UpdateAngularVelocity;
    
    b2Vec2 m_Force;
    b2Vec2 m_ForcePoint;
    bool m_UpdateForce;
    
    float m_Torque;
    bool m_UpdateTorque;
    
    b2Vec2 m_LinearImpulse;
    b2Vec2 m_LinearImpulsePoint;
    bool m_UpdateLinearImpulse;
    
    float m_AngularImpulse;
    bool m_UpdateAngularImpulse;
    
    float m_GravityScale;
    bool m_UpdateGravityScale;
    
    b2BodyType m_BodyType;
    bool m_UpdateBodyType;
};

#endif
