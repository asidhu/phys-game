
#include "opengl/primitives.h"
#include <cmath>
#include <string>
#define CIRCLE_RES 20
#define PIPERDEG 3.14159/180.0
const GLchar *simpleVertexShader =
	"void main(){"
		"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
	"}";
const GLchar *simpleFragmentShader =
	"uniform vec4 color;"
	"void main(){"
		"gl_FragColor = color;"
	"}";
const GLchar *textureVertexShader =
	"varying vec2 UV;"
	"void main(){"
		"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
		"UV=gl_Vertex+vec2(.5f,-.5f);"
		"UV.y*=-1;"
	"}";
const GLchar *textureFragmentShader =
	"uniform sampler2D myTexture;"
	"varying vec2 UV;"
	"void main(){"
		"gl_FragColor = texture(myTexture, UV);"
	"}";
int primitives::init(){
	//generate circle vbo
	glGenBuffers(1, &(this->circle_VBO));
	float circle_coords[(CIRCLE_RES+2)*2];
	circle_coords[0] = 0;
	circle_coords[1] = 0;
	for (int i = 1; i < CIRCLE_RES+2; i++){
		circle_coords[2 * i] = (float)cos(360. / CIRCLE_RES  * PIPERDEG *  i);
		circle_coords[2 * i + 1] = (float)sin(360. / CIRCLE_RES  * PIPERDEG *  i);
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->circle_VBO);
	glBufferData(GL_ARRAY_BUFFER, ((CIRCLE_RES+2) * 2)*sizeof(float), circle_coords, GL_STATIC_DRAW);

	//generate rect vbo
	float rect_coords[] = {
		0.f,0.f,
		-.5f,-.5f,
		.5f,-.5f,
		.5f,.5f,
		-.5f,.5f,
		-.5f,-.5f
	};
	glGenBuffers(1, &(this->rect_VBO));
	glBindBuffer(GL_ARRAY_BUFFER, this->rect_VBO);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), rect_coords, GL_STATIC_DRAW);

	//create a generic shader program
	this->simpleProgram = glCreateProgram();
	GLuint vertexShaderObj = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderObj = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShaderObj, 1, &simpleVertexShader, NULL);
	glShaderSource(fragmentShaderObj, 1, &simpleFragmentShader, NULL);
	glCompileShader(vertexShaderObj);
	glCompileShader(fragmentShaderObj);
	glAttachShader(this->simpleProgram, vertexShaderObj);
	glAttachShader(this->simpleProgram, fragmentShaderObj);
	glLinkProgram(this->simpleProgram);
	this->circleProgram_Color = glGetUniformLocation(this->simpleProgram, "color");

	//create a texture shader program
	this->textureProgram = glCreateProgram();
	vertexShaderObj = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderObj = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShaderObj, 1, &textureVertexShader, NULL);
	glShaderSource(fragmentShaderObj, 1, &textureFragmentShader, NULL);
	glCompileShader(vertexShaderObj);
	glCompileShader(fragmentShaderObj);
	glAttachShader(this->textureProgram, vertexShaderObj);
	glAttachShader(this->textureProgram, fragmentShaderObj);
	glLinkProgram(this->textureProgram);
	this->textureProgram_texUnit = glGetUniformLocation(this->textureProgram, "myTexture");
	return 1;
}

void primitives::deinit(){

}
float clamp(float in){
	if (in > 1)
		in = 1;
	if (in < 0)
		in = 0;
	return in;
}

void primitives::setColor(float r, float g, float b, float a){
	red = clamp(r);
	green = clamp(g);
	blue = clamp(b);
	alpha=clamp(a);
}

void primitives::setLineWidth(float w){
	lineWidth = w;
}

void primitives::drawCircle(float x, float y, float r){
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0);
	glScalef(r, r, 0);
	glEnable(GL_LINE_SMOOTH);
	glUseProgram(this->simpleProgram);
	glUniform4f(this->circleProgram_Color, red,green,blue,alpha);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->circle_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glLineWidth(lineWidth);
	glDrawArrays(GL_LINE_LOOP, 2, CIRCLE_RES);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
	glPopMatrix();
}
void primitives::drawLine(float x1,float y1, float x2, float y2){
	glPushMatrix();
	glLoadIdentity();
	glEnable(GL_LINE_SMOOTH);
	glUseProgram(this->simpleProgram);
	glUniform4f(this->circleProgram_Color, red, green, blue, alpha);
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
	glPopMatrix();
}
void primitives::drawRect(float x, float y, float w, float h, float rot){
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0);
	glRotatef(rot, 0, 0, 1);
	glScalef(w, h, 0);
	glEnable(GL_LINE_SMOOTH);
	glUseProgram(this->simpleProgram);
	glUniform4f(this->circleProgram_Color, red, green, blue, alpha);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->rect_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glLineWidth(lineWidth);
	glDrawArrays(GL_LINE_LOOP, 2, 4);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
	glPopMatrix();
}

void primitives::drawTexture(GLuint texID, float x, float y, float w, float h, float rot){
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0);
	glRotatef(-rot, 0, 0, 1);
	glScalef(w, h, 0);
	glUseProgram(this->textureProgram);
	glUniform1i(this->textureProgram_texUnit, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->rect_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void primitives::fillRect(float x, float y, float w, float h, float rot){
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0);
	glRotatef(-rot, 0, 0, 1);
	glScalef(w, h, 0);
	glUseProgram(this->simpleProgram);
	glUniform4f(this->circleProgram_Color, red, green, blue, alpha);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->rect_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
	glPopMatrix();
}
void primitives::fillCircle(float x, float y, float r){
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0);
	glScalef(r, r, 0);
	glUseProgram(this->simpleProgram);
	glUniform4f(this->circleProgram_Color, red, green, blue, alpha);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->circle_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_RES + 2);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
	glPopMatrix();
}