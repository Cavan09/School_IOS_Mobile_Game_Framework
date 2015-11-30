//
//  OpenGLFont.h
//  GameDevFramework
//
//  Created by Bradley Flood on 2012-09-18.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef OPENGL_FONT_H
#define OPENGL_FONT_H

#include "OpenGLColor.h"
#include "OpenGLTexture.h"
#include "OpenGLConstants.h"
#include <map>
#include <string>


typedef struct
{
  const char* fontName;
  float fontSize;
  const char* characterSet;
  OpenGLTexture* fontTexture;
  OpenGLTexture** fontFrames;
} OpenGLFontInfo;


class OpenGLTexture;

class OpenGLFont
{
public:
  OpenGLFont(const char* fontName, float fontSize = OPENGL_FONT_DEFAULT_SIZE, const char* characterSet = OPENGL_FONT_DEFAULT_CHARACTER_SET);
  ~OpenGLFont();
  
  void setText(const char* text);
  const char* getText();
  
  float getWidth();
  float getHeight();
  
  void setColor(OpenGLColor color);
  OpenGLColor getColor();
  
  void setAlpha(float alpha);
  float getAlpha();

private:
  friend class OpenGLRenderer;

  OpenGLTexture* fontTextureForCharacter(char character);
  OpenGLTexture* getFontTexture();

  OpenGLFontInfo* m_FontInfo;

  std::map<std::string, OpenGLTexture*> m_FontFramesMap;
  std::string m_CharacterSet;
  std::string m_Text;
  
  float m_Width;
  float m_Height;
  
  OpenGLColor m_Color;
};

#endif
