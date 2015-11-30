//
//  ResourceUtils.cpp
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "ResourceUtils.h"
#include "DeviceUtils.h"


namespace ResourceUtils
{
  const char* getFilenameForResource(const char* aFilename, const char* aFileExtension, bool aCheckForIPadVersion)
  {
    //Safety check that the resource key is not nil.
    if(aFilename != nil && aCheckForIPadVersion == true)
    {
      //Convert the c strings into NSStrings
      NSString *filename = [[NSString alloc] initWithCString:aFilename encoding:NSUTF8StringEncoding];
      NSString *fileExtension = [[NSString alloc] initWithCString:aFileExtension encoding:NSUTF8StringEncoding];
      NSMutableString *mutableFilename = nil;
    
      //Check if the device is either an iPad, we need to load the correct file if it is.
      if(DeviceUtils::isDeviceIPad() == true)
      {
        //The device is an iPad, we need to apped '-iPad' to the original filename
        mutableFilename = [[NSMutableString alloc] initWithString:filename];
        [mutableFilename appendString:@"-iPad"];
      
        //Check to make sure the iPad resource exits at the resource path
        if(doesFileExistsAtResourcePath(getPathForResource([mutableFilename UTF8String], aFileExtension, false)) == false)
        {
          [mutableFilename release];
          mutableFilename = nil;
        }
      }
      
      //Release the NSString objects
      [filename release];
      [fileExtension release];
      
      //Safety check that the mutable filename isn't nil
      if(mutableFilename != nil)
      {
        const char* returnFileName = [mutableFilename UTF8String];
        [mutableFilename release];
        return returnFileName;
      }
    }
    
    //Return the original filename
    return aFilename;
  }

  const char* getPathForResource(const char* aFilename, const char* aFileExtension, bool aCheckForIPadVersion)
  {
    //Convert the c strings into NSStrings
    NSString *filename = [[NSString alloc] initWithCString:getFilenameForResource(aFilename, aFileExtension, aCheckForIPadVersion) encoding:NSUTF8StringEncoding];
    NSString *fileExtension = [[NSString alloc] initWithCString:aFileExtension encoding:NSUTF8StringEncoding];

    //Get the resource path from the NSBundle
    NSString *path = [[NSBundle mainBundle] pathForResource:filename ofType:fileExtension];
    
    //Release the filename and fileExtension objects
    [filename release];
    [fileExtension release];
    
    //Return the path
    return [path UTF8String];
  }
  
  const char* getPathForPngResource(const char* aFilename)
  {
    return getPathForResource(aFilename, "png");
  }
  
  const char* getPathForPlistResource(const char* aFilename)
  {
    return getPathForResource(aFilename, "plist");
  }
    
  bool doesFileExistsAtResourcePath(const char* aPath)
  {
    //Flag to keep track if the file does exist
    bool doesExist = false;
  
    //Safety check that the path isn't NULL
    if(aPath != NULL)
    {
      //Convert the c string into a NSString
      NSString *path = [[NSString alloc] initWithCString:aPath encoding:NSUTF8StringEncoding];
      
      //Set the doesExist flag
      doesExist = [[NSFileManager defaultManager] fileExistsAtPath:path];
      
      //Release the path object
      [path release];
    }
    
    //Return wether the file exists or not
    return doesExist;
  }
}