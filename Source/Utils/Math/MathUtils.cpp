//
//  MathUtils.cpp
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "MathUtils.h"


unsigned int MathUtils::nextPowerOf2(unsigned int aValue)
{
  //Determine the power of 2 for the parameter
  unsigned int powerOf2 = 4;
  while(powerOf2 < aValue)
  {
    powerOf2 <<= 1;
  }
  
  return powerOf2;
}

float MathUtils::radiansToDegrees(float aRadians)
{
  return aRadians * (180.0f / M_PI);
}

float MathUtils::degressToRadians(float aDegrees)
{
  return aDegrees * (M_PI / 180.0f);
}
