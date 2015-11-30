//
//  b2Helper.h
//  GameDevFramework
//
//  Created by Bradley Flood on 2012-10-04.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef GameDevFramework_b2Helper_h
#define GameDevFramework_b2Helper_h

#include "Box2D.h"

//Macros to maintain compatibility with the textbook
#define PW2RW(value) b2Helper::box2dSpaceToScreenSpace(value)
#define RW2PW(value) b2Helper::screenSpaceToBox2dSpace(value)


class b2Helper
{
public:
  static float screenSpaceToBox2dSpace(float value);
  static b2Vec2 screenSpaceToBox2dSpace(float x, float y);
  static b2Vec2 screenSpaceToBox2dSpace(b2Vec2 value);
  
  static float box2dSpaceToScreenSpace(float value);
  static b2Vec2 box2dSpaceToScreenSpace(float x, float y);
  static b2Vec2 box2dSpaceToScreenSpace(b2Vec2 value);
  
  static float box2dRatio();
};

#endif
