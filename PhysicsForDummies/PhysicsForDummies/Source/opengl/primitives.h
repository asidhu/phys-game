#pragma once
#include "opengl/opengl.h"


class primitives{
private:
	GLuint rect_VBO;
	GLuint circle_VBO;
	GLuint simpleProgram;
	GLuint textureProgram;
	GLuint textureProgram_texUnit;
	GLuint circleProgram_Color;
	float red, green, blue, alpha;
	float lineWidth;
public:
	int init();
	void deinit();
	void setColor(float r, float g, float b, float a);
	void setLineWidth(float w);
	void drawRect(float x, float y, float w, float h, float rot = 0);
	void drawLine(float x, float y, float w, float h);
	void fillRect(float x, float y, float w, float h, float rot=0);
	void drawCircle(float x, float y, float r);
	void fillCircle(float x, float y, float r);
	void drawTexture(GLuint texID, float x, float y, float w = 1.f, float h = 1.f, float rot = 0.f);
};
