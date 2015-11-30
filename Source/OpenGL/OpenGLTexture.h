//
//  OpenGLTexture.h
//  GameDevFramework
//
//  Created by Brad Flood on 11-10-16.
//  Copyright 2011 Algonquin College. All rights reserved.
//

#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include <OpenGLES/ES1/glext.h>
#include "OpenGLColor.h"


typedef struct
{  
  GLuint sourceX;
  GLuint sourceY;
  GLuint sourceWidth;
  GLuint sourceHeight;
  GLuint textureWidth;
  GLuint textureHeight;
  GLenum textureFormat;
  GLuint textureId;
  const char* textureFilename;
} OpenGLTextureInfo;


class OpenGLTexture
{
public:
  OpenGLTexture(const char* filename, const char* atlasKey = NULL);
  OpenGLTexture(OpenGLTextureInfo* textureInfo);
  virtual ~OpenGLTexture();

	GLuint getFormat();
	GLuint getId();

	virtual GLuint getSourceX();
	virtual GLuint getSourceY();
	virtual GLuint getSourceWidth();
	virtual GLuint getSourceHeight();
  
	GLuint getTextureWidth();
	GLuint getTextureHeight();
  
  const char* getFilename();
  
  void setColor(OpenGLColor color);
  OpenGLColor getColor();
  
  void setAlpha(float alpha);
	float getAlpha();
  
  virtual bool isAnimated();
  
  void setAnchorPoint(float x, float y);
  float getAnchorPointX();
  float getAnchorPointY();

protected:
  friend class OpenGLFont;
  friend class OpenGLAnimatedTexture;
  friend class OpenGLTextureManager;

  void setTextureInfo(OpenGLTextureInfo* textureInfo);
  OpenGLTextureInfo* getTextureInfo();

  OpenGLTextureInfo* m_TextureInfo;
  OpenGLColor m_Color;
  float m_AnchorX;
  float m_AnchorY;
};

#endif
