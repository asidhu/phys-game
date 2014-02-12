
#include "opengl/primitives.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <gl/glew.h>
#include <gl/glfw.h>
#include <emscripten/emscripten.h>
#define CIRCLE_RES 60
#define PIPERDEG 3.14159/180.0
const GLchar *simpleVertexShader =
		"uniform mat4 mvpMat;		  \n"
		"attribute vec4 vPosition;    \n"
		"void main()                  \n"
		"{                            \n"
		"   gl_Position = mvpMat*vPosition;  \n"
		"}                            \n";
const GLchar *simpleFragmentShader =
		"precision mediump float;\n"\
		"void main()                                  \n"
		"{                                            \n"
		"  gl_FragColor = vec4 ( 1.0, 1.0, 1.0, 1.0 );\n"	
		"}                                            \n";

/**
* Multiplies two 4x4 matrices.
*
* The result is stored in matrix m.
*
* @param m the first matrix to multiply
* @param n the second matrix to multiply
*/
static void
multiply(GLfloat *m, const GLfloat *n)
{
	GLfloat tmp[16];
	const GLfloat *row, *column;
	div_t d;
	int i, j;

	for (i = 0; i < 16; i++) {
		tmp[i] = 0;
		d = div(i, 4);
		row = n + d.quot * 4;
		column = m + d.rem;
		for (j = 0; j < 4; j++)
			tmp[i] += row[j] * column[j * 4];
	}
	memcpy(m, &tmp, sizeof tmp);
}
/**
* Multiplies two 4x4 matrices.
*
* The result is stored in matrix m.
*
* @param m the first matrix to multiply
* @param n the second matrix to multiply
*/
static void
multiplyFlip(const GLfloat *m, GLfloat *n)
{
	GLfloat tmp[16];
	const GLfloat *row, *column;
	div_t d;
	int i, j;

	for (i = 0; i < 16; i++) {
		tmp[i] = 0;
		d = div(i, 4);
		row = n + d.quot * 4;
		column = m + d.rem;
		for (j = 0; j < 4; j++)
			tmp[i] += row[j] * column[j * 4];
	}
	memcpy(n, &tmp, sizeof tmp);
}
/**
* Rotates a 4x4 matrix.
*
* @param[in,out] m the matrix to rotate
* @param angle the angle to rotate
* @param x the x component of the direction to rotate to
* @param y the y component of the direction to rotate to
* @param z the z component of the direction to rotate to
*/
static void
rotate(GLfloat *m, GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	double s, c;

	sincos(angle, &s, &c);
	GLfloat r[16] = {
		x * x * (1 - c) + c, y * x * (1 - c) + z * s, x * z * (1 - c) - y * s, 0,
		x * y * (1 - c) - z * s, y * y * (1 - c) + c, y * z * (1 - c) + x * s, 0,
		x * z * (1 - c) + y * s, y * z * (1 - c) - x * s, z * z * (1 - c) + c, 0,
		0, 0, 0, 1
	};

	multiply(m, r);
}


/**
* Translates a 4x4 matrix.
*
* @param[in,out] m the matrix to translate
* @param x the x component of the direction to translate to
* @param y the y component of the direction to translate to
* @param z the z component of the direction to translate to
*/
static void
translate(GLfloat *m, GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat t[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x, y, z, 1 };

	multiply(m, t);
}
/**
* Scales a 4x4 matrix.
*
* @param[in,out] m the matrix to translate
* @param x the x component of the direction to translate to
* @param y the y component of the direction to translate to
* @param z the z component of the direction to translate to
*/
static void
scale(GLfloat *m, GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat t[16] = { x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0,0,0, 1 };

	multiply(m, t);
}
/**
* Creates an identity 4x4 matrix.
*
* @param m the matrix make an identity matrix
*/
static void
identity(GLfloat *m)
{
	GLfloat t[16] = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0,
	};

	memcpy(m, t, sizeof(t));
}

/**
* Transposes a 4x4 matrix.
*
* @param m the matrix to transpose
*/
static void
transpose(GLfloat *m)
{
	GLfloat t[16] = {
		m[0], m[4], m[8], m[12],
		m[1], m[5], m[9], m[13],
		m[2], m[6], m[10], m[14],
		m[3], m[7], m[11], m[15] };

	memcpy(m, t, sizeof(t));
}

/**
* Inverts a 4x4 matrix.
*
* This function can currently handle only pure translation-rotation matrices.
* Read http://www.gamedev.net/community/forums/topic.asp?topic_id=425118
* for an explanation.
*/
static void
invert(GLfloat *m)
{
	GLfloat t[16];
	identity(t);

	// Extract and invert the translation part 't'. The inverse of a
	// translation matrix can be calculated by negating the translation
	// coordinates.
	t[12] = -m[12]; t[13] = -m[13]; t[14] = -m[14];

	// Invert the rotation part 'r'. The inverse of a rotation matrix is
	// equal to its transpose.
	m[12] = m[13] = m[14] = 0;
	transpose(m);

	// inv(m) = inv(r) * inv(t)
	multiply(m, t);
}

/**
* Calculate a perspective projection transformation.
*
* @param m the matrix to save the transformation in
* @param fovy the field of view in the y direction
* @param aspect the view aspect ratio
* @param zNear the near clipping plane
* @param zFar the far clipping plane
*/
void perspective(GLfloat *m, GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
	GLfloat tmp[16];
	identity(tmp);

	double sine, cosine, cotangent, deltaZ;
	GLfloat radians = fovy / 2 * M_PI / 180;

	deltaZ = zFar - zNear;
	sincos(radians, &sine, &cosine);

	if ((deltaZ == 0) || (sine == 0) || (aspect == 0))
		return;

	cotangent = cosine / sine;

	tmp[0] = cotangent / aspect;
	tmp[5] = cotangent;
	tmp[10] = -(zFar + zNear) / deltaZ;
	tmp[11] = -1;
	tmp[14] = -2 * zNear * zFar / deltaZ;
	tmp[15] = 0;

	memcpy(m, tmp, sizeof(tmp));
}
/**
* Calculate a perspective projection transformation.
*
* @param m the matrix to save the transformation in
* @param fovy the field of view in the y direction
* @param aspect the view aspect ratio
* @param zNear the near clipping plane
* @param zFar the far clipping plane
*/
void ortho(GLfloat *m, GLfloat l, GLfloat r, GLfloat t, GLfloat b, GLfloat zNear, GLfloat zFar)
{
	GLfloat tmp[16];
	tmp[0] = 2.f / (r - l);
	tmp[3] = -(r + l) / (r - l);
	tmp[5] = 2.f / (t - b);
	tmp[7] = -(t + b) / (t - b);
	tmp[10] = -2.f / (zFar - zNear);
	tmp[11] = -(zFar + zNear) / (zFar - zNear);
	tmp[15] = 1;
	memcpy(m, tmp, sizeof(tmp));
}
GLuint LoadShader(GLenum type, const char *shaderSrc)
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader(type);

	if (shader == 0)
		return 0;

	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, NULL);

	// Compile the shader
	glCompileShader(shader);

	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			//char* infoLog = malloc(sizeof(char)* infoLen);

			//glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			//esLogMessage("Error compiling shader:\n%s\n", infoLog);

			//free(infoLog);
		}

		glDeleteShader(shader);
		return 0;
	}

	return shader;

}
GLfloat mvpMat[16];
GLfloat projMat[16];
int primitives::init(float l, float r, float t, float b){
	ortho(projMat, l, r, t, b, 1, -1);
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
	GLuint vertexShaderObj = LoadShader(GL_VERTEX_SHADER,simpleVertexShader);
	GLuint fragmentShaderObj = LoadShader(GL_FRAGMENT_SHADER,simpleFragmentShader);
	if (!vertexShaderObj || !fragmentShaderObj)
		return 0;
	glAttachShader(this->simpleProgram, vertexShaderObj);
	glAttachShader(this->simpleProgram, fragmentShaderObj);
	glLinkProgram(this->simpleProgram);
	this->circleProgram_Color = glGetUniformLocation(this->simpleProgram, "color");
	this->simpleProgram_Mat = glGetUniformLocation(this->simpleProgram, "mvpMat");

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
	identity(mvpMat);
	translate(mvpMat, x, y, 0);
	scale(mvpMat, r, r, 1);
	multiplyFlip(projMat, mvpMat);
	glEnable(GL_LINE_SMOOTH);
	glUseProgram(this->simpleProgram);
	glUniformMatrix4fv(this->simpleProgram_Mat, 1, GL_FALSE, mvpMat);
	glUniform4f(this->circleProgram_Color, red,green,blue,alpha);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->circle_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glLineWidth(lineWidth);
	glDrawArrays(GL_LINE_LOOP, 2, CIRCLE_RES);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
}

void primitives::drawRect(float x, float y, float w, float h, float rot){
	identity(mvpMat);
	translate(mvpMat, x, y, 0);
	scale(mvpMat, w, h, 1);
	multiplyFlip(projMat, mvpMat);
	glEnable(GL_LINE_SMOOTH);
	glUseProgram(this->simpleProgram);
	glUniformMatrix4fv(this->simpleProgram_Mat, 1, GL_FALSE, mvpMat);
	glUniform4f(this->circleProgram_Color, red, green, blue, alpha);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->rect_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glLineWidth(lineWidth);
	glDrawArrays(GL_LINE_LOOP, 2, 4);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
}

void primitives::fillRect(float x, float y, float w, float h, float rot){
	identity(mvpMat);
	translate(mvpMat, x, y, 0);
	scale(mvpMat, w, h, 1);
	multiplyFlip(projMat, mvpMat);
	glUseProgram(this->simpleProgram);
	glUniformMatrix4fv(this->simpleProgram_Mat, 1, GL_FALSE, mvpMat);
	glUniform4f(this->circleProgram_Color, red, green, blue, alpha);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->rect_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
}
void primitives::fillCircle(float x, float y, float r){
	identity(mvpMat);
	translate(mvpMat, x, y, 0);
	scale(mvpMat, r, r, 1);
	multiplyFlip(projMat, mvpMat);
	glUseProgram(this->simpleProgram);
	glUniformMatrix4fv(this->simpleProgram_Mat, 1, GL_FALSE, mvpMat);
	glUniform4f(this->circleProgram_Color, red, green, blue, alpha);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->circle_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_RES + 2);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
}