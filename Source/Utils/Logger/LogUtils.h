//
//  LogUtils.h
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef LOG_UTILS_H
#define LOG_UTILS_H

class Log
{
public:
	static void error(const char* output, ...);
	static void trace(const char* output, ...);
	static void debug(const char* output, ...);
	static void custom(const char* custom, const char* output, ...);
  
	static void enable();
	static void disable();

private:
	static void output(const char* label, const char* output, va_list argumentsList);
    static bool m_IsEnabled;
};

#endif
