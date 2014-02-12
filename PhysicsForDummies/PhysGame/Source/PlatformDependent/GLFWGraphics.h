#pragma once
#include "PhysGame\Source\PlatformDependent\PlatformGraphics.h"

class GLFWwindow;
class GLFWGraphics :public PlatformGraphics{
public:
	GLFWwindow* m_window;
	bool m_initialized;
	
	GLFWGraphics(int w, int h);


};