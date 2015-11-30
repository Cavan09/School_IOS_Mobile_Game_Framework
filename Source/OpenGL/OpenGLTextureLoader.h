//
//  OpenGLTextureLoader.h
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef OPENGL_TEXTURE_LOADER_H
#define OPENGL_TEXTURE_LOADER_H

#include "OpenGLTexture.h"
#include "OpenGLAnimatedTexture.h"

namespace OpenGLTextureLoader
{
  void loadTextureFromPath(const char* path, OpenGLTextureInfo** textureInfo);
  void loadTextureFromImage(void* image, OpenGLTextureInfo** textureInfo);
    
  void loadTextureFromAtlas(const char* pngPath, const char* plistPath, const char* atlasKey, OpenGLTextureInfo** textureInfo);
    
  void loadAnimatedTextureFromPath(const char* path, const char* plistPath, OpenGLAnimatedTextureInfo** animatedTextureInfo);
}

#endif
