//
//  ResourceUtils.h
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef RESOURCE_UTILS_H
#define RESOURCE_UTILS_H

namespace ResourceUtils
{
  const char* getFilenameForResource(const char* filename, const char* fileExtension, bool checkForIPadVersion = true);

  const char* getPathForResource(const char* filename, const char* fileExtension, bool checkForIPadVersion = true);
  const char* getPathForPngResource(const char* filename);
  const char* getPathForPlistResource(const char* filename);

  bool doesFileExistsAtResourcePath(const char* path);
}

#endif
