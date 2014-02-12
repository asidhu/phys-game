#pragma once
#include "PhysGame\Source\PlatformDependent\PlatformInput.h"

class GLFWwindow;
class GLFWInput :public PlatformInput{
	GLFWwindow* m_window;
	GLFWInput(GLFWwindow* win);

	void pollInput();
};