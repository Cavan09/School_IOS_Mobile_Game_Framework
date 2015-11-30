//
//  FileUtils.cpp
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "FileUtils.h"


namespace FileUtils
{
  void saveString(const char* aString, const char* aKey)
  {
    if(aString != NULL && aKey != NULL)
    {
      NSString *string = [[NSString alloc] initWithCString:aString encoding:NSUTF8StringEncoding];
      NSString *key = [[NSString alloc] initWithCString:aKey encoding:NSUTF8StringEncoding];
      [[NSUserDefaults standardUserDefaults] setObject:string forKey:key];
      [[NSUserDefaults standardUserDefaults] synchronize];
      [string release];
      [key release];
    }
  }

  void saveInt(int aValue, const char* aKey)
  {
    if(aKey != NULL)
    {
      NSString *key = [[NSString alloc] initWithCString:aKey encoding:NSUTF8StringEncoding];
      [[NSUserDefaults standardUserDefaults] setInteger:aValue forKey:key];
      [[NSUserDefaults standardUserDefaults] synchronize];
      [key release];
    }
  }
  
  void saveFloat(float aValue, const char* aKey)
  {
    if(aKey != NULL)
    {
      NSString *key = [[NSString alloc] initWithCString:aKey encoding:NSUTF8StringEncoding];
      [[NSUserDefaults standardUserDefaults] setFloat:aValue forKey:key];
      [[NSUserDefaults standardUserDefaults] synchronize];
      [key release];
    }
  }
  
  void saveDouble(double aValue, const char* aKey)
  {
    if(aKey != NULL)
    {
      NSString *key = [[NSString alloc] initWithCString:aKey encoding:NSUTF8StringEncoding];
      [[NSUserDefaults standardUserDefaults] setDouble:aValue forKey:key];
      [[NSUserDefaults standardUserDefaults] synchronize];
      [key release];
    }
  }

  void saveBool(bool aValue, const char* aKey)
  {
    if(aKey != NULL)
    {
      NSString *key = [[NSString alloc] initWithCString:aKey encoding:NSUTF8StringEncoding];
      [[NSUserDefaults standardUserDefaults] setBool:aValue forKey:key];
      [[NSUserDefaults standardUserDefaults] synchronize];
      [key release];
    }
  }
  
  const char* loadString(const char* aKey)
  {
    const char* returnString = NULL;
    if(aKey != NULL)
    {
      NSString *key = [[NSString alloc] initWithCString:aKey encoding:NSUTF8StringEncoding];
      returnString = [[[NSUserDefaults standardUserDefaults] stringForKey:key] UTF8String];
      [key release];
    }
    return returnString;
  }
  
  int saveInt(const char* aKey)
  {
    int returnInt = -1;
    if(aKey != NULL)
    {
      NSString *key = [[NSString alloc] initWithCString:aKey encoding:NSUTF8StringEncoding];
      returnInt = [[NSUserDefaults standardUserDefaults] integerForKey:key];
      [key release];
    }
    return returnInt;
  }
  
  float saveFloat(const char* aKey)
  {
    float returnFloat = -1.0f;
    if(aKey != NULL)
    {
      NSString *key = [[NSString alloc] initWithCString:aKey encoding:NSUTF8StringEncoding];
      returnFloat = [[NSUserDefaults standardUserDefaults] floatForKey:key];
      [key release];
    }
    return returnFloat;
  }
  
  double saveDouble(const char* aKey)
  {
    double returnDouble = -1.0;
    if(aKey != NULL)
    {
      NSString *key = [[NSString alloc] initWithCString:aKey encoding:NSUTF8StringEncoding];
      returnDouble = [[NSUserDefaults standardUserDefaults] doubleForKey:key];
      [key release];
    }
    return returnDouble;
  }
  
  bool saveBool(const char* aKey)
  {
    bool returnBool = false;
    if(aKey != NULL)
    {
      NSString *key = [[NSString alloc] initWithCString:aKey encoding:NSUTF8StringEncoding];
      returnBool = [[NSUserDefaults standardUserDefaults] boolForKey:key];
      [key release];
    }
    return returnBool;
  }
}