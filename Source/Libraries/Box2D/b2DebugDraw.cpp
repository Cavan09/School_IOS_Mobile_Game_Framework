//
//  b2DebugDraw.cpp
//  GameDevFramework
//
//  Created by Bradley Flood on 2012-10-03.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "b2DebugDraw.h"
#include "OpenGL.h"


b2DebugDraw::b2DebugDraw(float aRatio)
	: m_Ratio(aRatio)
{

}

void b2DebugDraw::DrawPolygon(const b2Vec2* aVertices, int32 aVertexCount, const b2Color& aColor)
{
  int vertexSize = 2;
	float vertices[aVertexCount * vertexSize];
	for(int i = 0; i < aVertexCount * vertexSize; i+=vertexSize)
  {
    int index = i / vertexSize;
		b2Vec2 temp = aVertices[index];
		temp *= m_Ratio;
		vertices[i] = temp.x;
		vertices[i+1] = temp.y;
	}
  
  OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorRGB(aColor.r, aColor.g, aColor.b));
  OpenGLRenderer::getInstance()->drawPolygon(GL_LINE_LOOP, (float*)vertices, vertexSize, aVertexCount);
}

void b2DebugDraw::DrawSolidPolygon(const b2Vec2* aVertices, int32 aVertexCount, const b2Color& aColor)
{
  int vertexSize = 2;
	float vertices[aVertexCount * vertexSize];
	for(int i = 0; i < aVertexCount * vertexSize; i+=vertexSize)
  {
    int index = i / vertexSize;
		b2Vec2 temp = aVertices[index];
		temp *= m_Ratio;
		vertices[i] = temp.x;
		vertices[i+1] = temp.y;
	}

  OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorRGBA(aColor.r * 0.5f, aColor.g * 0.5f, aColor.b * 0.5f, 0.5f));
  OpenGLRenderer::getInstance()->drawPolygon(GL_TRIANGLE_FAN, (float*)vertices, vertexSize, aVertexCount);
  OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorRGB(aColor.r, aColor.g, aColor.b));
  OpenGLRenderer::getInstance()->drawPolygon(GL_LINE_LOOP, (float*)vertices, vertexSize, aVertexCount);
}

void b2DebugDraw::DrawCircle(const b2Vec2& aCenter, float32 aRadius, const b2Color& aColor)
{
  OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorRGB(aColor.r, aColor.g, aColor.b));
  OpenGLRenderer::getInstance()->drawCircle(aCenter.x * m_Ratio, aCenter.y * m_Ratio, aRadius * m_Ratio, false, 36);
}

void b2DebugDraw::DrawSolidCircle(const b2Vec2& aCenter, float32 aRadius, const b2Vec2& aAxis, const b2Color& aColor)
{
  OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorRGBA(aColor.r * 0.5f, aColor.g * 0.5f, aColor.b * 0.5f, 0.5f));
  OpenGLRenderer::getInstance()->drawCircle(aCenter.x * m_Ratio, aCenter.y * m_Ratio, aRadius * m_Ratio, true, 36);
  OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorRGB(aColor.r, aColor.g, aColor.b));
  OpenGLRenderer::getInstance()->drawCircle(aCenter.x * m_Ratio, aCenter.y * m_Ratio, aRadius * m_Ratio, false, 36);
  DrawSegment(aCenter,aCenter + aRadius * aAxis, aColor);
}

void b2DebugDraw::DrawSegment(const b2Vec2& aP1, const b2Vec2& aP2, const b2Color& aColor)
{
  OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorRGB(aColor.r, aColor.g, aColor.b));
  OpenGLRenderer::getInstance()->drawLine(aP1.x * m_Ratio, aP1.y * m_Ratio, aP2.x * m_Ratio, aP2.y * m_Ratio);
}

void b2DebugDraw::DrawTransform(const b2Transform& xf)
{
	b2Vec2 p1 = xf.p, p2;
	const float32 k_axisScale = 0.4f;

	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	DrawSegment(p1,p2,b2Color(1,0,0));
	
	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	DrawSegment(p1,p2,b2Color(0,1,0));
}
