//
//  b2Helper.cpp
//  GameDevFramework
//
//  Created by Bradley Flood on 2012-10-04.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "b2Helper.h"
#include "DeviceUtils.h"
#include "Constants.h"


float b2Helper::screenSpaceToBox2dSpace(float aValue)
{
  return aValue / b2Helper::box2dRatio();
}

b2Vec2 b2Helper::screenSpaceToBox2dSpace(float aX, float aY)
{
  return b2Vec2(b2Helper::screenSpaceToBox2dSpace(aX), b2Helper::screenSpaceToBox2dSpace(aY));
}

b2Vec2 b2Helper::screenSpaceToBox2dSpace(b2Vec2 aValue)
{
  return b2Vec2(b2Helper::screenSpaceToBox2dSpace(aValue.x), b2Helper::screenSpaceToBox2dSpace(aValue.y));
}

float b2Helper::box2dSpaceToScreenSpace(float aValue)
{
  return aValue * b2Helper::box2dRatio();
}

b2Vec2 b2Helper::box2dSpaceToScreenSpace(float aX, float aY)
{
  return b2Vec2(b2Helper::box2dSpaceToScreenSpace(aX), b2Helper::box2dSpaceToScreenSpace(aY));
}

b2Vec2 b2Helper::box2dSpaceToScreenSpace(b2Vec2 aValue)
{
  return b2Vec2(b2Helper::box2dSpaceToScreenSpace(aValue.x), b2Helper::box2dSpaceToScreenSpace(aValue.y));
}

float b2Helper::box2dRatio()
{
  static float contentScaleFactor = -1.0f;
  if(contentScaleFactor < 0.0f)
  {
    contentScaleFactor = DeviceUtils::getContentScaleFactor();
  }
  return GAME_PHYSICS_PIXELS_TO_METERS_RATIO * contentScaleFactor;
}
