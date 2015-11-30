//
//  OpenGLFont.cpp
//  GameDevFramework
//
//  Created by Bradley Flood on 2012-09-18.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "OpenGLFont.h"
#include "OpenGLTextureManager.h"


OpenGLFont::OpenGLFont(const char* aFontName, float aFontSize, const char* aCharacterSet) :
  m_FontInfo(NULL),
  m_CharacterSet(""),
  m_Text(""),
  m_Width(0.0f),
  m_Height(0.0f),
  m_Color(OpenGLColorBlack())
{
  //Allocate the font info struct
  m_FontInfo = (OpenGLFontInfo*)malloc(sizeof(OpenGLFontInfo));
  memset(m_FontInfo, 0, sizeof(OpenGLFontInfo));

  //Set the font name and size
  m_FontInfo->fontName = aFontName;
  m_FontInfo->fontSize = aFontSize;

  //Set the character set
  m_CharacterSet.assign(aCharacterSet);
  
  //Sort the character set
  std::sort(m_CharacterSet.begin(), m_CharacterSet.end());
  
  //Get the c string version of the character set and assign it to the font info
  m_FontInfo->characterSet = m_CharacterSet.c_str();
  
  //Allocate the array for the font frames
  m_FontInfo->fontFrames = new OpenGLTexture *[m_CharacterSet.length()];
  
  //Load the font
  OpenGLTextureManager::getInstance()->loadFont(&m_FontInfo);
  
  //Cycle through and setup the font frames map
  for(int index = 0; index < m_CharacterSet.length(); index++)
  {
    std::string characterIndex(m_CharacterSet.substr(index, 1));
    m_FontFramesMap[characterIndex] = m_FontInfo->fontFrames[index];
  }
  
  //Set the font color
  setColor(OpenGLColorBlack());
}

OpenGLFont::~OpenGLFont()
{
  //Safety check the font info pointer
  if(m_FontInfo != NULL)
  {
    //Safety check the font info font frames pointer
    if(m_FontInfo->fontFrames != NULL)
    {
      //Cycle through and delete all the font frames
      for(int i = 0; i < m_CharacterSet.length(); i++)
      {
        m_FontInfo->fontFrames[i]->setTextureInfo(NULL);
        delete m_FontInfo->fontFrames[i];
        m_FontInfo->fontFrames[i] = NULL;
      }
      
      //Delete the font frames array
      delete [] m_FontInfo->fontFrames;
      m_FontInfo->fontFrames = NULL;
    }
    
    //Delete the font texture
    if(m_FontInfo->fontTexture != NULL)
    {
      delete m_FontInfo->fontTexture;
      m_FontInfo->fontTexture = NULL;
    }
    
    //Lastly free the font info struct
    free(m_FontInfo);
    m_FontInfo = NULL;
  }
}

OpenGLTexture* OpenGLFont::fontTextureForCharacter(char aCharacter)
{
  std::string characterIndex(1, aCharacter);
  return m_FontFramesMap[characterIndex];
}

OpenGLTexture* OpenGLFont::getFontTexture()
{
  return m_FontInfo->fontTexture;
}

void OpenGLFont::setText(const char* aText)
{
  //Set the new text
  m_Text.assign(aText);
  
  //Reset the width and height
  m_Width = 0.0f;
  m_Height = 0.0f;
  
  //Safety check that there is text to process, then
  //calculate the width and height of the text
  if(m_Text.length() > 0)
  {
    for(int index = 0; index < m_Text.length(); index++)
    {
      OpenGLTexture* fontTexture = fontTextureForCharacter(m_Text.c_str()[index]);
      if(fontTexture != NULL)
      {
        m_Width += fontTexture->getSourceWidth();
        m_Height = fmaxf(m_Height, fontTexture->getSourceHeight());
      }
    }
  }
}

const char* OpenGLFont::getText()
{
  return m_Text.c_str();
}

float OpenGLFont::getWidth()
{
  return m_Width;
}

float OpenGLFont::getHeight()
{
  return m_Height;
}

void OpenGLFont::setColor(OpenGLColor aColor)
{
  //Set the color of the text
  m_Color = aColor;
  
  //Cycle through each character, setting each one's color
  for(int index = 0; index < m_CharacterSet.length(); index++)
  {
    OpenGLTexture* fontTexture = m_FontInfo->fontFrames[index];
    fontTexture->setColor(m_Color);
  }
}

OpenGLColor OpenGLFont::getColor()
{
  return m_Color;
}

void OpenGLFont::setAlpha(float aAlpha)
{
  OpenGLColor color = getColor();
  color.alpha = aAlpha;
  setColor(color);
}

float OpenGLFont::getAlpha()
{
  return getColor().alpha;
}
