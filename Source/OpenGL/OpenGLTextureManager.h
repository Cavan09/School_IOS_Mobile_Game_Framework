//
//  OpenGLTextureManager.h
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef OPENGL_TEXTURE_MANAGER_H
#define OPENGL_TEXTURE_MANAGER_H

#include "OpenGLFont.h"
#include "OpenGLTexture.h"
#include "OpenGLAnimatedTexture.h"
#include <map>


class OpenGLTextureManager
{
public:
  static OpenGLTextureManager* getInstance();
  
  void loadFont(OpenGLFontInfo** fontInfo);
  void loadTexture(const char* filename, OpenGLTextureInfo** textureInfo);
  void loadTextureFromAtlas(const char* filename, const char* atlasKey, OpenGLTextureInfo** textureInfo);
  void loadAnimatedTexture(const char* filename, OpenGLAnimatedTextureInfo** animatedTextureInfo);
  
  void unloadTexture(OpenGLTextureInfo* textureInfo);
  void unloadAnimatedTexture(OpenGLAnimatedTextureInfo* animatedTextureInfo);

private:
  OpenGLTextureManager();
  ~OpenGLTextureManager();

  //Singleton instance of OpenGLTextureManager
  static OpenGLTextureManager* m_Instance;

  //Private struct to keep track of the OpenGL id and its retain count.
  typedef struct TextureIdRetainInfo
  {
    unsigned int textureId;
    int retainCount;
  }TextureIdRetainInfo;

  //Compare filenames function for the texture retain map
  struct compareFilenames
  {
    bool operator()(char const* filenameA, char const* filenameB)
    {
      return std::strcmp(filenameA, filenameB) < 0;
    }
  };
  
  //Map to keep track of the OpenGL texture's loaded into memory, this prevents us from loading the same texture into memory twice
  std::map<const char*, TextureIdRetainInfo, compareFilenames> m_TextureIdRetainMap;
};

#endif
