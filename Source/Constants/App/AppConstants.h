//
//  AppConstants.h
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef APP_CONSTANTS_H
#define APP_CONSTANTS_H

enum
{
	TouchEventBegan = 0,
    TouchEventEnded,
    TouchEventMoved,
    TouchEventCancelled
};
typedef unsigned int TouchEvent;

enum
{
	FramePerSecond60 = 1,
	FramePerSecond30 = 2
};
typedef unsigned int FramePerSecond;

extern const FramePerSecond TARGET_FRAMES_PER_SECOND;

#endif
