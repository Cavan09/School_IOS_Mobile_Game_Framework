//
//  LogUtils.cpp
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "LogUtils.h"


bool Log::m_IsEnabled = true;

void Log::error(const char* aOutput, ...)
{
  va_list arguments;
  va_start(arguments, aOutput);
  output("ERROR", aOutput, arguments);
  va_end (arguments);
}

void Log::trace(const char* aOutput, ...)
{
  va_list arguments;
  va_start(arguments, aOutput);
  output("TRACE", aOutput, arguments);
  va_end(arguments);
}

void Log::debug(const char* aOutput, ...)
{
  va_list arguments;
  va_start(arguments, aOutput);
  output("DEBUG", aOutput, arguments);
  va_end(arguments);
}

void Log::custom(const char* aCustom, const char* aOutput, ...)
{
  va_list arguments;
  va_start(arguments, aOutput);
  output(aCustom, aOutput, arguments);
  va_end(arguments);
}

void Log::output(const char* aLabel, const char* aOutput, va_list aArgumentsList)
{
#if DEBUG
  if(m_IsEnabled == true)
  {
    printf("[%s] - ", aLabel);
    vprintf(aOutput, aArgumentsList);
    printf("\n");
  }
#endif
}

void Log::enable()
{
  m_IsEnabled = true;
}

void Log::disable()
{
  m_IsEnabled = false;
}