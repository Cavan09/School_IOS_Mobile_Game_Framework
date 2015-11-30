//
//  Player.cpp
//  GameDevFramework
//
//  Created by Cavan Macphail on 2014-04-19.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Player.h"
#include "../Utils/Device/DeviceUtils.h"

Player::Player(float x, float y) : GameObject()
{
    m_Texture = new OpenGLTexture("Images/Dragon");
    m_Texture->setAnchorPoint(0.5f,0.5f);
    CreateDragon(x, y);
    reset();
}

Player::~Player()
{
    if(m_Texture != NULL)
    {
        delete m_Texture;
        m_Texture = NULL;
    }
}

void Player::update(double delta)
{
    
}
void Player::paint()
{
    
}

void Player::reset()
{
    m_Score = 0;
}

const char* Player::getTye()
{
    return "Player";
}

void Player::toucheEvent(TouchEvent aTouchEvent, float locationX, float loactionY, float previousX, float previousY)
{
    //if(aTouchEvent == TouchEventBegan)
}

int Player::getScore()
{
    return m_Score;
}

void Player::setScore(int score)
{
    m_Score = score;
}

void Player::CreateDragon(float x, float y)
{
    //convert x and y to box2d space
    b2Vec2 posiition  = b2Helper::screenSpaceToBox2dSpace(x, y);
    b2Vec2 size = b2Helper::screenSpaceToBox2dSpace(DeviceUtils::getScreenResolutionWidth() * 0.1f, DeviceUtils::getScreenResolutionHeight() * 0.01f);
    m_Body = Game::getInstance()->createBox(posiition, size, b2_kinematicBody);
    m_Body->SetUserData(this);
}