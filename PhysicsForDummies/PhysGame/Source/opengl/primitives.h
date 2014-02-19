#pragma once
#include "opengl/opengl.h"


struct draw_square{
	float x, y, w, h;
} ;

class primitives{
private:
	GLuint rect_VBO;
	GLuint circle_VBO;
	GLuint simpleProgram;
	GLuint textureProgram;
	GLuint textureProgram_texCoord;
	GLuint textureProgram_texUnit;
	GLuint circleProgram_Color;
	float red, green, blue, alpha;
	float lineWidth;
#ifdef EMSCRIPTEN
	GLuint simpleProgram_Mat;
#endif

public:
	int init();
	void deinit();
	void setupViewport(float l, float r, float t, float b);
	void setColor(float r, float g, float b, float a);
	void setLineWidth(float w);
	void drawRect(float x, float y, float w, float h, float rot = 0);
	void drawLine(float x, float y, float w, float h);
	void fillRect(float x, float y, float w, float h, float rot=0);
	void drawCircle(float x, float y, float r);
	void fillCircle(float x, float y, float r);
	void drawTexture(GLuint texID, float x, float y, const float texcoords[8], float w = 1.f, float h = 1.f, float rot = 0.f);
	void drawStenciledTexture(GLuint texID, draw_square&,draw_square&, const float texcoords[8], float rot = 0.f);
};
