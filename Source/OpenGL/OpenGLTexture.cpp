//
//  OpenGLTexture.h
//  GameDevFramework
//
//  Created by Brad Flood on 11-10-16.
//  Copyright 2011 Algonquin College. All rights reserved.
//

#include "OpenGLTexture.h"
#include "OpenGLTextureManager.h"



OpenGLTexture::OpenGLTexture(const char* aFilename, const char* aAtlasKey) :
  m_TextureInfo(NULL),
  m_Color(OpenGLColorWhite()),
  m_AnchorX(0.0f),
  m_AnchorY(0.0f)
{
  //Safety check that the filename isn't NULL
  if(aFilename != NULL)
  {
    //Allocate the texture info struct
    m_TextureInfo = (OpenGLTextureInfo*)malloc(sizeof(OpenGLTextureInfo));
    memset(m_TextureInfo, 0, sizeof(OpenGLTextureInfo));

    //Set the texture info filename
    m_TextureInfo->textureFilename = (const char*)malloc(strlen(aFilename)+1);
    memcpy((void*)m_TextureInfo->textureFilename, aFilename, strlen(aFilename)+1);

    //Load the texture
    if(aAtlasKey != NULL)
    {
      OpenGLTextureManager::getInstance()->loadTextureFromAtlas(aFilename, aAtlasKey, &m_TextureInfo);
    }
    else
    {
      OpenGLTextureManager::getInstance()->loadTexture(aFilename, &m_TextureInfo);
    }
  }
}

OpenGLTexture::OpenGLTexture(OpenGLTextureInfo* aTextureInfo) :
  m_TextureInfo(NULL),
  m_Color(OpenGLColorWhite()),
  m_AnchorX(0.0f),
  m_AnchorY(0.0f)
{
  //Safety check that the texture info isn't NULL
  if(aTextureInfo != NULL)
  {
    //Allocate the texture info struct
    m_TextureInfo = (OpenGLTextureInfo*)malloc(sizeof(OpenGLTextureInfo));
    memset(m_TextureInfo, 0, sizeof(OpenGLTextureInfo));

    //Copy the contents of the Texture info struct
    memcpy(m_TextureInfo, aTextureInfo, sizeof(OpenGLTextureInfo));
  }
}

OpenGLTexture::~OpenGLTexture()
{
  //Unload the OpenGL texture from memory
  OpenGLTextureManager::getInstance()->unloadTexture(m_TextureInfo);

  //Free the texture info struct
  if(m_TextureInfo != NULL)
  {
    if(m_TextureInfo->textureFilename != NULL)
    {
      free((void*)m_TextureInfo->textureFilename);
      m_TextureInfo->textureFilename = NULL;
    }
  
    free(m_TextureInfo);
    m_TextureInfo = NULL;
  }
}

GLuint OpenGLTexture::getFormat()
{
  return m_TextureInfo->textureFormat;
}

GLuint OpenGLTexture::getId()
{
  return m_TextureInfo->textureId;
}

GLuint OpenGLTexture::getSourceX()
{
  return m_TextureInfo->sourceX;
}

GLuint OpenGLTexture::getSourceY()
{
  return m_TextureInfo->sourceY;
}

GLuint OpenGLTexture::getSourceWidth()
{
  return m_TextureInfo->sourceWidth;
}

GLuint OpenGLTexture::getSourceHeight()
{
  return m_TextureInfo->sourceHeight;
}

GLuint OpenGLTexture::getTextureWidth()
{
  return m_TextureInfo->textureWidth;
}

GLuint OpenGLTexture::getTextureHeight()
{
  return m_TextureInfo->textureHeight;
}
  
const char* OpenGLTexture::getFilename()
{
  return m_TextureInfo->textureFilename;
}

void OpenGLTexture::setColor(OpenGLColor aColor)
{
  m_Color = aColor;
}

OpenGLColor OpenGLTexture::getColor()
{
  return m_Color;
}

void OpenGLTexture::setAlpha(float aAlpha)
{
  OpenGLColor color = getColor();
  color.alpha = aAlpha;
  setColor(color);
}

float OpenGLTexture::getAlpha()
{
  return getColor().alpha;
}

bool OpenGLTexture::isAnimated()
{
  return false;
}

void OpenGLTexture::setAnchorPoint(float aX, float aY)
{
  m_AnchorX = aX;
  m_AnchorY = aY;
}

float OpenGLTexture::getAnchorPointX()
{
  return m_AnchorX;
}

float OpenGLTexture::getAnchorPointY()
{
  return m_AnchorY;
}

void OpenGLTexture::setTextureInfo(OpenGLTextureInfo* aTextureInfo)
{
  m_TextureInfo = aTextureInfo;
}

OpenGLTextureInfo* OpenGLTexture::getTextureInfo()
{
  return m_TextureInfo;
}
