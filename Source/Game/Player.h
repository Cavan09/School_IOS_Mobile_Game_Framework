//
//  Player.h
//  GameDevFramework
//
//  Created by Cavan Macphail on 2014-04-19.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDevFramework__Player__
#define __GameDevFramework__Player__

#include <iostream>
#include "GameObject.h"

class Player : public GameObject
{
public:
    Player(float x, float y);
    ~Player();
    
    void update(double delta);
    void paint();
    void reset();
    
    const char* getTye();
    void toucheEvent(TouchEvent aTouchEvent, float locationX, float locationY, float previousX, float previousY);
    
    int getScore();
    void setScore(int score);
    
private:
    
    void CreateDragon(float x, float y);
    
    float m_ProjVelocity;
    float m_X;
    float m_Y;
    int m_Score;
    b2Vec2 m_Position;
    
};

#endif /* defined(__GameDevFramework__Player__) */
