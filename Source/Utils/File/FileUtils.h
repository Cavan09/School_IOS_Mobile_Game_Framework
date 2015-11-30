//
//  FileUtils.h
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

namespace FileUtils
{
  //Saving functions
  void saveString(const char* string, const char* key);
  void saveInt(int value, const char* key);
  void saveFloat(float value, const char* key);
  void saveDouble(double value, const char* key);
  void saveBool(bool value, const char* key);
  
  //Loading functions
  const char* loadString(const char* key);
  int saveInt(const char* key);
  float saveFloat(const char* key);
  double saveDouble(const char* key);
  bool saveBool(const char* key);
}

#endif
