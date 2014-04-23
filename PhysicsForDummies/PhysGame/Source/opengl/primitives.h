#pragma once
#include <libs\glm\mat4x4.hpp>
#include <libs\glm\gtc\matrix_transform.hpp>
#include <libs\glm\gtc\type_ptr.hpp>
#include "opengl/opengl.h"
#define MAX_BATCH 256
#define MAX_TEX 7
struct draw_square{
	float x, y, w, h;
} ;

class primitives{
private:

	GLint vAttribsEnabled[10];
	GLint vAttribDivisor[10];
	glm::mat4 worldMat;
	GLuint rect_VBO;
	GLuint circle_VBO;
	GLuint simpleProgram;
	GLuint circleInstanceProgram;
	GLuint rectInstanceProgram;
	GLuint texInstanceProgram;
	GLuint uberShaderProgram;
	struct{
		GLuint instanceBuffer;
		GLuint worldMat;
	} riData;
	struct {
		GLuint instanceBuffer;
		GLuint worldMat;
	} ciData;
	struct{
		GLuint texID;
		GLuint worldMat;
		GLuint instanceBuffer;
	} tiData;
	struct{
		GLuint color;
		GLuint wMat, mMat;
	} spData;
	
	GLuint textureProgram;
	GLuint textureProgram_texCoord;
	GLuint textureProgram_texUnit;
	
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
	void setAttribDivisor(GLint index, GLint target);
	void drawRect(float x, float y, float w, float h, float rot = 0);
	void drawLine(float x, float y, float w, float h);
	void fillRect(float x, float y, float w, float h, float rot=0);
	void drawCircle(float x, float y, float r);
	void batchDrawCircle(int num, void* data);//x,y,radius,r,g,b,a,z
	void batchDrawSquare(int num, void* data);//x,y,rot,w,h,r,g,b,a,z
	void batchCircle(int num, void* data); //x,y,radius,r,g,b,a,z
	void batchSquare(int num, void* data); //x,y,rot,w,h,r,g,b,a,z
	void batchSquareTexture(int num, GLint *texIDs, int numTexs, void* data);//x,y,rot,sx,sy,tx,ty,texid,z

	void fillCircle(float x, float y, float r);
	void drawTexture(GLuint texID, float x, float y, const float texcoords[8], float w = 1.f, float h = 1.f, float rot = 0.f);
	void drawStenciledTexture(GLuint texID, draw_square&,draw_square&, const float texcoords[8], float rot = 0.f);
};
