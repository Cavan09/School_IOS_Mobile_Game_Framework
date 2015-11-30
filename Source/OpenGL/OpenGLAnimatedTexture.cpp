//
//  OpenGLAnimatedTexture.h
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "OpenGLAnimatedTexture.h"
#include "OpenGLTextureManager.h"


OpenGLAnimatedTexture::OpenGLAnimatedTexture(const char* aFilename, bool aDoesLoop, float aFrameSpeed) : OpenGLTexture(NULL, NULL),
  m_AnimatedTextureInfo(NULL),
  m_Listener(NULL),
  m_FrameSpeed(aFrameSpeed),
  m_ElapsedTime(0.0),
  m_FrameIndex(0),
  m_DoesLoop(aDoesLoop)
{
  if(aFilename != NULL)
  {
    //Allocate the animated texture info struct
    m_AnimatedTextureInfo = (OpenGLAnimatedTextureInfo*)malloc(sizeof(OpenGLAnimatedTextureInfo));
    memset(m_AnimatedTextureInfo, 0, sizeof(OpenGLAnimatedTextureInfo));
    
    //Allocate the animated texture info struct's texture info struct
    m_AnimatedTextureInfo->textureInfo = (OpenGLTextureInfo*)malloc(sizeof(OpenGLTextureInfo));
    memset(m_AnimatedTextureInfo->textureInfo, 0, sizeof(OpenGLTextureInfo));
    
    //Set the texture info filename
    m_AnimatedTextureInfo->textureInfo->textureFilename = (const char*)malloc(strlen(aFilename)+1);
    memcpy((void*)m_AnimatedTextureInfo->textureInfo->textureFilename, aFilename, strlen(aFilename)+1);
    
    
    
    //Load the animated texture
    OpenGLTextureManager::getInstance()->loadAnimatedTexture(aFilename, &m_AnimatedTextureInfo);
    
    //Set the frame index to zero, (the first frame)
    setFrameIndex(0);
  }
}

OpenGLAnimatedTexture::~OpenGLAnimatedTexture()
{
  //Set the textureInfo struct to NULL, (this way OpenGLTexture won't try to free it)
  m_TextureInfo = NULL;
  
  //Set the listener to NULL
  m_Listener = NULL;
  
  //Unload the animated texture
  OpenGLTextureManager::getInstance()->unloadAnimatedTexture(m_AnimatedTextureInfo);
  
  //Safety check the animated texture info struct
  if(m_AnimatedTextureInfo != NULL)
  {
    //Safety check the animated texture info frames array
    if(m_AnimatedTextureInfo->frames != NULL)
    {
      //Cycle through and delete all the frames in the animated texture
      for(int i = 0; i < m_AnimatedTextureInfo->frameCount; i++)
      {
        m_AnimatedTextureInfo->frames[i]->setTextureInfo(NULL);
        delete m_AnimatedTextureInfo->frames[i];
        m_AnimatedTextureInfo->frames[i] = NULL;
      }
      
      //Delete the animated texture frames array
      delete [] m_AnimatedTextureInfo->frames;
      m_AnimatedTextureInfo->frames = NULL;
    }
    
    //Free the texture info struct in the animated texture info struct
    if(m_AnimatedTextureInfo->textureInfo != NULL)
    {
      if(m_AnimatedTextureInfo->textureInfo->textureFilename != NULL)
      {
        free((void*)m_AnimatedTextureInfo->textureInfo->textureFilename);
        m_AnimatedTextureInfo->textureInfo->textureFilename = NULL;
      }
    
      free(m_AnimatedTextureInfo->textureInfo);
      m_AnimatedTextureInfo->textureInfo = NULL;
    }
    
    //Free the animated texture info struct
    free(m_AnimatedTextureInfo);
    m_AnimatedTextureInfo = NULL;
  }
}

void OpenGLAnimatedTexture::update(double aDelta)
{
  //Increment the elapsed time.
  m_ElapsedTime += aDelta;
  
  //Calculate the new frame index.
  int lastFrameIndex = m_FrameIndex;
	int frameIndex = (int)(m_ElapsedTime / (1.0 / m_FrameSpeed));
  
  //If the animation loops, we need to see if it is ready wrap around to the start.
  if(m_DoesLoop == true)
  {
    frameIndex = frameIndex % getFrameCount();
  }
  
  //If animation is completed, and the listener is not NULL, call the listener.
  if(frameIndex >= getFrameCount()) 
  {
    if(m_Listener != NULL)
    {
      m_Listener->animatedTextureDidFinishAnimating(this);
    }
  }
  
  //If the frame index has changed, set the new frame index.
  if(frameIndex != lastFrameIndex)
  {
    setFrameIndex(frameIndex);
  }
}

void OpenGLAnimatedTexture::reset()
{
  m_ElapsedTime = 0.0;
  setFrameIndex(0);
}

void OpenGLAnimatedTexture::setListener(OpenGLAnimatedTextureListener* aListener)
{
  m_Listener = aListener;
}

unsigned int OpenGLAnimatedTexture::getFrameCount()
{
  return m_AnimatedTextureInfo->frameCount;
}


int OpenGLAnimatedTexture::getFrameIndex()
{
  return m_FrameIndex;
}

void OpenGLAnimatedTexture::setFrameIndex(int aFrameIndex)
{
  //Safety check the frame index.
  if(aFrameIndex >= 0 && aFrameIndex < getFrameCount())
  {
    //Set the frames index.
    m_FrameIndex = aFrameIndex;
    
    //Set the elapsed time member.
    m_ElapsedTime = (double)m_FrameIndex * (1.0 / m_FrameSpeed);
    
    //Set the image info with the current frame's image info.
    OpenGLTexture* frameInfo = m_AnimatedTextureInfo->frames[m_FrameIndex];
    m_TextureInfo = frameInfo->getTextureInfo();
  }
}

bool OpenGLAnimatedTexture::getDoesLoop()
{
  return m_DoesLoop;
}

void OpenGLAnimatedTexture::setDoesLoop(bool aDoesLoop)
{
  m_DoesLoop = aDoesLoop;
}

bool OpenGLAnimatedTexture::isAnimated()
{
  return true;
}
