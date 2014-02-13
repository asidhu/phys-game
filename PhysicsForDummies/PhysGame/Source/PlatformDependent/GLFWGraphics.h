#pragma once
#include "PhysGame\Source\PlatformDependent\PlatformGraphics.h"


struct GLFWwindow;
class GLFWGraphics :public PlatformGraphics{
private:
	float m_left, m_right;
public:
	GLFWwindow* m_window;
	bool m_initialized;
	int m_win_width, m_win_height;
	GLFWGraphics(int w, int h);
	void drawList(RenderList*);
	void start();
	void close();
	float getLeft(){
		return m_left;
	}

	float getRight(){
		return m_right;
	}
	float getWidth(){
		return m_left - m_right;
	}
};