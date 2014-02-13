#pragma once
#include "PhysGame\Source\PlatformDependent\PlatformInput.h"

struct GLFWwindow;
class GLFWInput :public PlatformInput{
	GLFWwindow* m_window;
public:
	GLFWInput(GLFWwindow* win);

	void pollInput();
};