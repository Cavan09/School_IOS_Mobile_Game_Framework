//
//  OpenGLRenderer.h
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef OPENGL_RENDERER_H
#define OPENGL_RENDERER_H

#include "OpenGLColor.h"
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

class OpenGLTexture;
class OpenGLFont;

class OpenGLRenderer
{
public:
    static OpenGLRenderer* getInstance();
    static void cleanupInstance();
    
    void clear();
    
    OpenGLColor getBackgroundColor();
    void setBackgroundColor(OpenGLColor backgroundColor);
    
    OpenGLColor getForegroundColor();
    void setForegroundColor(OpenGLColor foregroundColor);
    
    OpenGLFont* getDefaultFont();
    
    void enableClip(float x, float y, float width, float height);
    void disableClip();
    
    void enableBlending(GLenum sourceBlending = GL_SRC_ALPHA, GLenum destinationBlending = GL_ONE_MINUS_SRC_ALPHA);
    void disableBlending();
    
    float getPointSize();
    void setPointSize(float pointSize);
    void resetPointSize();
    
    float getLineWidth();
    void setLineWidth(float lineWidth);
    void resetLineWidth();
    
    void drawPoint(float x, float y);
    void drawLine(float x1, float y1, float x2, float y2);
    
    void drawCircle(float centerX, float centerY, float radius, bool isFilled = true, int lineSegments = 36);
    void drawRectangle(float x, float y, float width, float height, bool isFilled = true);
    
    void drawPolygon(GLenum renderMode, float* vertices, int vertexSize, int vertexCount);
    void drawPolygon(GLenum renderMode, float* vertices, int vertexSize, int vertexCount, float* colors, int colorSize);
    
	void drawTexture(OpenGLTexture* texture, float x, float y, float angle = 0.0f);
	void drawTexture(OpenGLTexture* texture, float x, float y, float width, float height, float angle = 0.0f);
	void drawTexture(OpenGLTexture* texture, float* uvCoordinates, float* vertices);
    
    void drawFont(OpenGLFont* font, float x, float y);
    
private:
    OpenGLRenderer();
    ~OpenGLRenderer();
    
    static OpenGLRenderer* m_Instance;
    OpenGLColor m_BackgroundColor;
    OpenGLColor m_ForegroundColor;
    OpenGLFont* m_DefaultFont;
};

#endif
