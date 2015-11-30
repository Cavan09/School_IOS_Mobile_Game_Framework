//
//  DeviceUtils.h
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef DEVICE_UTILS_H
#define DEVICE_UTILS_H

namespace DeviceUtils
{
    void getScreenResolution(float& width, float& height, bool scaled = true);
    float getScreenResolutionWidth(bool scaled = true);
    float getScreenResolutionHeight(bool scaled = true);
    float getContentScaleFactor();
    
    bool hasRetinaDisplay();
    bool isDeviceIPad();
    bool isDeviceSimulator();
    
    bool isOrientationPortrait();
    bool isOrientationLandscape();
    
    const char* getName();
    const char* getModel();
    const char* getSystemName();
    const char* getSystemVersion();
    
    unsigned long long getMemorySize();
    bool hasLowOnMemorySize();
    bool hasDualCoreCPU();
}

#endif
