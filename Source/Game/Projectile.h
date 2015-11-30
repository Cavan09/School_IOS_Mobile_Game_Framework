//
//  Projectile.h
//  GameDevFramework
//
//  Created by Cavan Macphail on 2014-04-19.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDevFramework__Projectile__
#define __GameDevFramework__Projectile__

#include "GameObject.h"


class Projectile : public GameObject
{
public:
    Projectile(b2Vec2 position, float angle, b2Vec2 linearVelocity);
    ~Projectile();
    
    const char* getType();
};

#endif /* defined(__GameDevFramework__Projectile__) */
