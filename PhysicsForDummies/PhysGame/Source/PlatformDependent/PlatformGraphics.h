#pragma once
#include "PhysGame\Source\RenderList.h"

class PlatformGraphics{
public:
	float m_top, m_bottom, m_centerX;
	virtual float getLeft()=0;
	virtual float getRight()=0;
	virtual float getWidth() = 0;
	virtual void start() = 0;
	virtual void close() = 0;
	virtual void drawList(RenderList*) = 0;
};