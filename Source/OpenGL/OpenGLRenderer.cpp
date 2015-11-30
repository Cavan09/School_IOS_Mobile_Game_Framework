//
//  OpenGLRenderer.cpp
//  GameDevFramework
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "OpenGLRenderer.h"
#include "OpenGLTexture.h"
#include "OpenGLFont.h"
#include "DeviceUtils.h"


OpenGLRenderer* OpenGLRenderer::m_Instance = NULL;

OpenGLRenderer* OpenGLRenderer::getInstance()
{
    if(m_Instance == NULL)
    {
        m_Instance = new OpenGLRenderer();
    }
    
    return m_Instance;
}

void OpenGLRenderer::cleanupInstance()
{
    if(m_Instance != NULL)
    {
        delete m_Instance;
        m_Instance = NULL;
    }
}

OpenGLRenderer::OpenGLRenderer()
{
    setBackgroundColor(OpenGLColorCornFlowerBlue());
    setForegroundColor(OpenGLColorBlack());
    m_DefaultFont = new OpenGLFont(OPENGL_FONT_DEFAULT_FONT, OPENGL_FONT_DEFAULT_SIZE * DeviceUtils::getContentScaleFactor(), OPENGL_FONT_EXTENDED_CHARACTER_SET);
}

OpenGLRenderer::~OpenGLRenderer()
{
    if(m_DefaultFont != NULL)
    {
        delete m_DefaultFont;
        m_DefaultFont = NULL;
    }
}

void OpenGLRenderer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

OpenGLColor OpenGLRenderer::getBackgroundColor()
{
    return m_BackgroundColor;
}

void OpenGLRenderer::setBackgroundColor(OpenGLColor aBackgroundColor)
{
    m_BackgroundColor = aBackgroundColor;
    glClearColor(m_BackgroundColor.red, m_BackgroundColor.green, m_BackgroundColor.blue, m_BackgroundColor.alpha);
}

OpenGLColor OpenGLRenderer::getForegroundColor()
{
    return m_ForegroundColor;
}

void OpenGLRenderer::setForegroundColor(OpenGLColor aForegroundColor)
{
    m_ForegroundColor = aForegroundColor;
}

OpenGLFont* OpenGLRenderer::getDefaultFont()
{
    return m_DefaultFont;
}

void OpenGLRenderer::enableClip(float x, float y, float width, float height)
{
    //Enable the scissor clipping.
    glEnable(GL_SCISSOR_TEST);
    glScissor(x, y, width, height);
}

void OpenGLRenderer::disableClip()
{
    glDisable(GL_SCISSOR_TEST);
}

void OpenGLRenderer::enableBlending(GLenum aSourceBlending, GLenum aDestinationBlending)
{
    glBlendFunc(aSourceBlending, aDestinationBlending);
    glEnable(GL_BLEND);
}

void OpenGLRenderer::disableBlending()
{
    glDisable(GL_BLEND);
}

float OpenGLRenderer::getPointSize()
{
    float pointSize;
    glGetFloatv(GL_POINT_SIZE, &pointSize);
    return pointSize;
}

void OpenGLRenderer::setPointSize(float aPointSize)
{
	glPointSize(aPointSize);
}

void OpenGLRenderer::resetPointSize()
{
    setPointSize(1.0f);
}

float OpenGLRenderer::getLineWidth()
{
    float lineWidth;
    glGetFloatv(GL_LINE_WIDTH, &lineWidth);
    return lineWidth;
}

void OpenGLRenderer::setLineWidth(float aLineWidth)
{
    glLineWidth(aLineWidth);
}

void OpenGLRenderer::resetLineWidth()
{
    setLineWidth(1.0f);
}

void OpenGLRenderer::drawPoint(float aX, float aY)
{
	float vertices[] = { aX, aY };
	int vertexSize = 2;
	int vertexCount = 1;
	drawPolygon(GL_POINTS, vertices, vertexSize, vertexCount);
}

void OpenGLRenderer::drawLine(float aX1, float aY1, float aX2, float aY2)
{
	float vertices[] = { aX1, aY1, aX2, aY2 };
	int vertexSize = 2;
	int vertexCount = 2;
	drawPolygon(GL_LINES, vertices, vertexSize, vertexCount);
}

void OpenGLRenderer::drawCircle(float aCenterX, float aCenterY, float aRadius, bool aIsFilled, int aLineSegments)
{
	int vertexSize = 2;
	int vertexCount = aLineSegments;
	float vertices[vertexCount * vertexSize];
    
	float rotationAmount = (360.0f / aLineSegments);
    
	int count = 0;
	for (float i = 0; i < 359.99f; i+= rotationAmount)
	{
		vertices[count++] = aCenterX + (cosf((M_PI * i / 180.0)) * aRadius);
        vertices[count++] = aCenterY + (sinf((M_PI * i / 180.0)) * aRadius);
	}
    
	drawPolygon(aIsFilled ? GL_TRIANGLE_FAN : GL_LINE_LOOP, vertices, vertexSize, vertexCount);
}

void OpenGLRenderer::drawRectangle(float aX, float aY, float aWidth, float aHeight, bool aIsFilled)
{
	int vertexSize = 2;
	int vertexCount = 4;
	float vertices[vertexSize * vertexCount];
    
	vertices[0] = aX;          vertices[1] = aY;
	vertices[2] = aX + aWidth; vertices[3] = aY;
	vertices[4] = aX + aWidth; vertices[5] = aY + aHeight;
	vertices[6] = aX;          vertices[7] = aY + aHeight;
    
	drawPolygon(aIsFilled ? GL_TRIANGLE_FAN : GL_LINE_LOOP, vertices, vertexSize, vertexCount);
}

void OpenGLRenderer::drawPolygon(GLenum aRenderMode, float* aVertices, int aVertexSize, int aVertexCount)
{
	//Setup the colors array based on the foreground color
	int colorSize = 4;
	float colors[colorSize * aVertexCount];
	int count = 0;
	for (int i = 0; i < aVertexCount; ++i)
	{
		colors[count++] = m_ForegroundColor.red;
        colors[count++] = m_ForegroundColor.green;
        colors[count++] = m_ForegroundColor.blue;
        colors[count++] = m_ForegroundColor.alpha;
	}
    
	//If the foreground alpha isn't full, enable blending
	if(m_ForegroundColor.alpha != 1.0f)
	{
		enableBlending();
	}
    
	//Draw the polygon
	drawPolygon(aRenderMode, aVertices, aVertexSize, aVertexCount, colors, colorSize);
    
	//If the foreground alpha isn't full, blending is enabled, disable it
	if(m_ForegroundColor.alpha != 1.0f)
	{
		disableBlending();
	}
}

void OpenGLRenderer::drawPolygon(GLenum aRenderMode, float* aVertices, int aVertexSize, int aVertexCount, float* aColors, int aColorSize)
{
	//Enable the vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(aVertexSize, GL_FLOAT, 0, aVertices);
    
	//Enable the color array
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(aColorSize, GL_FLOAT, 0, aColors);
    
	//Render the polygon.
	glDrawArrays(aRenderMode, 0, aVertexCount);
    
	//Disable the vertex array client state.
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void OpenGLRenderer::drawTexture(OpenGLTexture* aTexture, float aX, float aY, float aAngle)
{
    if(aTexture != NULL)
    {
        drawTexture(aTexture, aX, aY, aTexture->getSourceWidth(), aTexture->getSourceHeight(), aAngle);
    }
}

void OpenGLRenderer::drawTexture(OpenGLTexture* aTexture, float aX, float aY, float aWidth, float aHeight, float aAngle)
{
	//Safety check the texture
	if(aTexture != NULL)
	{
        float uvCoordinates[8];
		float vertices[8];
        
		//Calculate the x and y values for the uv coordinates
		float x1 = (float)aTexture->getSourceX() / (float)aTexture->getTextureWidth();
		float y1 = 1.0f - ((float)(aTexture->getSourceY() + aTexture->getSourceHeight()) / (float)aTexture->getTextureHeight());
        float x2 = (float)(aTexture->getSourceX() + aTexture->getSourceWidth()) / (float)aTexture->getTextureWidth();
		float y2 = 1.0f - ((float)aTexture->getSourceY() / (float)aTexture->getTextureHeight());
        
		//Set the uvCoordinates
		uvCoordinates[0] = x1; uvCoordinates[1] = y1;
		uvCoordinates[2] = x2; uvCoordinates[3] = y1;
		uvCoordinates[4] = x1; uvCoordinates[5] = y2;
		uvCoordinates[6] = x2; uvCoordinates[7] = y2;
        
		//Set the vertices
		vertices[0] = 0.0f;           vertices[1] = 0.0f + aHeight;
		vertices[2] = 0.0f + aWidth;  vertices[3] = 0.0f + aHeight;
		vertices[4] = 0.0f;           vertices[5] = 0.0f;
		vertices[6] = 0.0f + aWidth; 	vertices[7] = 0.0f;
        
        //Push the Matrix
        glPushMatrix();
        
        //Translate the texture
        glTranslatef(aX - (aTexture->getSourceWidth() * aTexture->getAnchorPointX()), aY - (aTexture->getSourceHeight() * aTexture->getAnchorPointY()), 0.0f);
        
        //Rotate the texture
        if(aAngle != 0.0f)
        {
            //Rotate around the center of the texture.
            glTranslatef(aWidth/2, aHeight/2, 0.0f);
            glRotatef(aAngle, 0.0f, 0.0f, 1.0f);
            glTranslatef(-aWidth/2, -aHeight/2, 0.0f);
        }
        
		//Draw the texture
		drawTexture(aTexture, uvCoordinates, vertices);
        
        //Pop the Matrix
        glPopMatrix();
    }
}

void OpenGLRenderer::drawTexture(OpenGLTexture* aTexture, float* aUvCoordinates, float* aVertices)
{
	if(aTexture != NULL)
	{
		int vertexCount = 4;
		int vertexSize = 2;
        
		//Setup the colors array based on the foreground color
		bool hasTransparency = aTexture->getFormat() == GL_RGBA || aTexture->getAlpha() != 1.0f;
		int colorSize = hasTransparency ? 4 : 3;
		float colors[colorSize * vertexCount];
		int count = 0;
		for (int i = 0; i < vertexCount; ++i)
		{
			colors[count++] = aTexture->getColor().red;
			colors[count++] = aTexture->getColor().green;
			colors[count++] = aTexture->getColor().blue;
			if(colorSize == 4)
			{
				colors[count++] = aTexture->getAlpha();
			}
		}
        
		//If the foreground alpha isn't full, enable blending
		if(hasTransparency == true)
		{
			enableBlending();
		}
        
        //Set the uvCoordinates of the texture.
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, aUvCoordinates);
        
		//Enable the texture and bind it
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, aTexture->getId());
        
		//Draw the texture
		drawPolygon(GL_TRIANGLE_STRIP, aVertices, vertexSize, vertexCount, colors, colorSize);
        
        //Disable the client states
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
        
		//If the foreground alpha isn't full, blending is enabled, disable it
		if(hasTransparency == true)
		{
			disableBlending();
		}
	}
}

void OpenGLRenderer::drawFont(OpenGLFont* aFont, float aX, float aY)
{
    if(aFont != NULL)
    {
        //Used to keep track of the current character's x position.
        float startingX = aX;
        const char* text = aFont->getText();
        int length = strlen(text);
        
        //Cycle through and draw each character in the font's text.
        for(int i = 0; i < length; i++)
        {
            //Get the current character to render.
            char character = text[i];
            
            //Get the Font Texture for the character.
            OpenGLTexture *fontTexture = aFont->fontTextureForCharacter(character);
            if(fontTexture != NULL)
            {
                //Set the font texture's position, then draw it.
                drawTexture(fontTexture, startingX, aY);
                startingX += fontTexture->getSourceWidth();
            }
        }
    }
}
