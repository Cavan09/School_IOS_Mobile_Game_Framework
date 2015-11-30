//
//  DeviceUtils.cpp
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#import "DeviceUtils.h"
#import <sys/sysctl.h>
#import <mach/mach.h>
#import <mach/mach_host.h>

namespace DeviceUtils
{
  void getScreenResolution(float& aWidth, float& aHeight, bool aScaled)
  {
    //Get the width and height from the application size
    CGRect applicationSize = [[UIScreen mainScreen] applicationFrame];
    aWidth = applicationSize.size.width;
    aHeight = applicationSize.size.height;
    
    //If the device is in landscape mode, make sure the proper oriented resolution gets returned.
    if(isOrientationLandscape() == true)
    {
      aWidth = applicationSize.size.width > applicationSize.size.height ? applicationSize.size.width : applicationSize.size.height;
      aHeight = applicationSize.size.width > applicationSize.size.height ? applicationSize.size.height : applicationSize.size.width;
    }
    
    //If the scaled flag is true, multiply the size by the content scale factor
    if(aScaled == true)
    {
      aWidth *= getContentScaleFactor();
      aHeight *= getContentScaleFactor();
    }
  }
  
  float getScreenResolutionWidth(bool scaled)
  {
    float width, height;
    getScreenResolution(width, height, scaled);
    return width;
  }
  
  float getScreenResolutionHeight(bool scaled)
  {
    float width, height;
    getScreenResolution(width, height, scaled);
    return height;
  }
  
  float getContentScaleFactor()
  {
    static float contentScaleFactor = 0.0f;
    if(contentScaleFactor == 0.0f)
    {
      if([UIScreen instancesRespondToSelector:@selector(scale)])
      {
        contentScaleFactor = [[UIScreen mainScreen] scale];
      }
      else
      {
        contentScaleFactor = 1.0f;
      }
    }
    return contentScaleFactor;
  }
  
  bool hasRetinaDisplay()
  {
    return getContentScaleFactor() == 2.0f;
  }
  
  bool isDeviceIPad()
  {
    return UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad;
  }
  
  bool isDeviceSimulator()
  {
    NSRange range = [[[UIDevice currentDevice] model] rangeOfString:@"Simulator"];
    return range.location != NSNotFound;
  }
  
  bool isOrientationPortrait()
  {
    return isOrientationLandscape() == false;
  }
  
  bool isOrientationLandscape()
  {
    return (UIDeviceOrientationIsLandscape([[UIDevice currentDevice] orientation]) == YES || UIInterfaceOrientationIsLandscape([[UIApplication sharedApplication] statusBarOrientation]) == YES);
  }
  
  const char* getName()
  {
    return [[[UIDevice currentDevice] name] cStringUsingEncoding:NSUTF8StringEncoding];
  }
  
  const char* getModel()
  {
    return [[[UIDevice currentDevice] model] cStringUsingEncoding:NSUTF8StringEncoding];
  }
  
  const char* getSystemName()
  {
    return [[[UIDevice currentDevice] systemName] cStringUsingEncoding:NSUTF8StringEncoding];
  }
  
  const char* getSystemVersion()
  {
    return [[[UIDevice currentDevice] systemVersion] cStringUsingEncoding:NSUTF8StringEncoding];
  }
  
  unsigned long long getMemorySize()
  {
    static uint64_t memorySize = 0;
    if(memorySize == 0)
    {
      int mib[] = { CTL_HW, HW_MEMSIZE };
      size_t length = sizeof(memorySize);
      sysctl(mib, 2, &memorySize, &length, NULL, 0);
    }
    return memorySize;
  }
  
  bool hasLowOnMemorySize()
  {
    return getMemorySize() <= 268435456;
  }
  
  bool hasDualCoreCPU()
  {
    static unsigned int numberOfCPUs = 0;
    if(numberOfCPUs == 0)
    {
      size_t length = sizeof(numberOfCPUs);
      sysctlbyname ("hw.ncpu", &numberOfCPUs, &length, NULL, 0);
    }
    return numberOfCPUs > 1;
  }
}