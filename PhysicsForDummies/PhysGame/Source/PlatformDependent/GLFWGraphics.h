#pragma once
#include "PhysGame\Source\PlatformDependent\PlatformGraphics.h"

#include <map>

struct GLFWwindow;
class GLFWGraphics :public PlatformGraphics{
private:
	float m_left, m_right;
	std::map<unsigned int, unsigned int> m_resourceMap;
	void renderBatchCircle(bool hollow, std::list<RenderItem*> &list);
	void renderBatchSquare(bool hollow, std::list<RenderItem*> &list);
	void renderBatchTextureSquare(RenderList*);
public:
	GLFWwindow* m_window;
	bool m_initialized;
	int m_fb_width, m_fb_height;
	int m_win_width, m_win_height;
	GLFWGraphics(int w, int h);
	void drawList(RenderList*);
	void start();
	void close();
	void loadImage(unsigned int resID, char* filename);
	float getLeft(){
		return m_left;
	}

	float getRight(){
		return m_right;
	}
	float getWidth(){
		return  m_right- m_left;
	}
};