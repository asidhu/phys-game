#pragma once
#include "PhysGame\Source\RenderList.h"

class PlatformGraphics{
public:
	float m_top, m_bottom, m_centerX;
	void setupViewport(float t, float b, float cx){
		m_top = t;
		m_bottom = b;
		m_centerX = cx;
	}
	virtual float getLeft()=0;
	virtual float getRight()=0;
	virtual float getWidth() = 0;
	virtual void start() = 0;
	virtual void close() = 0;
	virtual void drawList(RenderList*) = 0;
	virtual void loadImage(unsigned int resID, const char* filename)=0;
	virtual void freeImage(unsigned int id) = 0;
};