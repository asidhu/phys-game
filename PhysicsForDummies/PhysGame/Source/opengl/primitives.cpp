


#ifdef EMSCRIPTEN
#include "opengl/primitives.h"
#include <cmath>
#include <cstdlib>
#include <cstring>
#define CIRCLE_RES 20
#define PIPERDEG 3.14159/180.0
const GLchar *simpleVertexShader =
"uniform mat4 mvpMat;		  \n"
"attribute vec4 vPosition;    \n"
"void main()                  \n"
"{                            \n"
"   gl_Position = mvpMat*vPosition;  \n"
"}                            \n";

const GLchar *simpleFragmentShader =
"precision mediump float;\n"
"uniform vec4 color;"
"void main()                                  \n"
"{                                            \n"
"  gl_FragColor = color;						\n"
"}                                            \n";

const GLchar *textureVertexShader =
"uniform mat4 mvpMat;		  \n"
"attribute vec4 vPosition;    \n"
"varying vec2 UV;				\n"
"void main(){					\n"
	"gl_Position = mvpMat*vPosition;"
	"UV=gl_Vertex+vec2(.5f,-.5f);"
	"UV.y*=-1;"
"}";

const GLchar *textureFragmentShader =
"uniform sampler2D myTexture;"
"varying vec2 UV;"
"void main(){"
	"gl_FragColor = texture2D(myTexture, UV);"
"}";
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
	GLfloat t[16] = { x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1 };

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
	float a = 2.f / (r - l),
		be=2.f / (t - b),
		c=-2.f / (zFar - zNear);
	float tx = -(r + l) / (r - l),
		ty=-(t + b) / (t - b),
		tz=-(zFar + zNear) / (zFar - zNear);

	GLfloat tmp[16] = {
		a, 0, 0, 0,
		0, be, 0, 0,
		0 ,0, c, 0,
		tx,ty,tz,1
	};
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
int primitives::init(){
	//generate circle vbo
	glGenBuffers(1, &(this->circle_VBO));
	float circle_coords[(CIRCLE_RES + 2) * 2];
	circle_coords[0] = 0;
	circle_coords[1] = 0;
	for (int i = 1; i < CIRCLE_RES + 2; i++){
		circle_coords[2 * i] = (float)cos(360. / CIRCLE_RES  * PIPERDEG *  i);
		circle_coords[2 * i + 1] = (float)sin(360. / CIRCLE_RES  * PIPERDEG *  i);
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->circle_VBO);
	glBufferData(GL_ARRAY_BUFFER, ((CIRCLE_RES + 2) * 2)*sizeof(float), circle_coords, GL_STATIC_DRAW);

	//generate rect vbo
	float rect_coords[] = {
		0.f, 0.f,
		-.5f, -.5f,
		.5f, -.5f,
		.5f, .5f,
		-.5f, .5f,
		-.5f, -.5f
	};
	glGenBuffers(1, &(this->rect_VBO));
	glBindBuffer(GL_ARRAY_BUFFER, this->rect_VBO);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), rect_coords, GL_STATIC_DRAW);

	//create a generic shader program
	this->simpleProgram = glCreateProgram();
	GLuint vertexShaderObj = LoadShader(GL_VERTEX_SHADER, simpleVertexShader);
	GLuint fragmentShaderObj = LoadShader(GL_FRAGMENT_SHADER, simpleFragmentShader);
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
	alpha = clamp(a);
}

void primitives::setupViewport(float l, float r, float t, float b){
	ortho(projMat,l,r,t,b,1.f,-1.f);
}


void primitives::setLineWidth(float w){
	lineWidth = w;
}

void primitives::drawCircle(float x, float y, float r){
	identity(mvpMat);
	translate(mvpMat, x, y, 0);
	scale(mvpMat, r, r, 1);
	multiplyFlip(projMat, mvpMat);
	//glEnable(GL_LINE_SMOOTH);
	glUseProgram(this->simpleProgram);
	glUniformMatrix4fv(this->simpleProgram_Mat, 1, GL_FALSE, mvpMat);
	glUniform4f(this->circleProgram_Color, red, green, blue, alpha);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->circle_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	//glLineWidth(lineWidth);
	glDrawArrays(GL_LINE_LOOP, 2, CIRCLE_RES);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
}

void primitives::drawRect(float x, float y, float w, float h, float rot){
	identity(mvpMat);
	translate(mvpMat, x, y, 0);
	rotate(mvpMat, rot * PIPERDEG, 0,0,1);
	scale(mvpMat, w, h, 1);
	multiplyFlip(projMat, mvpMat);
	//glEnable(GL_LINE_SMOOTH);
	glUseProgram(this->simpleProgram);
	glUniformMatrix4fv(this->simpleProgram_Mat, 1, GL_FALSE, mvpMat);
	glUniform4f(this->circleProgram_Color, red, green, blue, alpha);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->rect_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	//glLineWidth(lineWidth);
	glDrawArrays(GL_LINE_LOOP, 2, 4);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
}

void primitives::fillRect(float x, float y, float w, float h, float rot){
	identity(mvpMat);
	translate(mvpMat, x, y, 0);
	rotate(mvpMat, rot * PIPERDEG, 0,0,1);
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



void primitives::drawStenciledTexture(GLuint texID, draw_square& mask, draw_square& texture, const float texcoords[8], float rot){
	//draw mask
	setColor(1,1,1,1);
	drawRect(mask.x,mask.y,mask.w,mask.h,rot);
	return;
	/*
	glEnable(GL_STENCIL_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	glStencilFunc(GL_NEVER, 1, 0xFF);
	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
	glStencilMask(0xFF);
	glClear(GL_STENCIL_BUFFER_BIT);

	//draw mask.
	identity(mvpMat);
	translate(mvpMat, mask.x, mask.y, 0);
	rotate(mvpMat, rot, 0,0,1);
	scale(mvpMat, mask.w, mask.h, 1);
	multiplyFlip(projMat, mvpMat);
	glUseProgram(this->simpleProgram);
	glUniformMatrix4fv(this->simpleProgram_Mat, 1, GL_FALSE, mvpMat);
	glUniform4f(this->circleProgram_Color, red, green, blue, alpha);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->rect_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableVertexAttribArray(0);
	glUseProgram(0);



	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
	glStencilMask(0x00);
	glStencilFunc(GL_EQUAL, 1, 0xFF);

	//draw texture
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	identity(mvpMat);
	translate(mvpMat, mask.x, mask.y, 0);
	if (mask.w>mask.h)
		scale(mvpMat, mask.w*2, mask.w*2, 1);
	else
		scale(mvpMat, mask.h*2, mask.h*2, 1);
	multiplyFlip(projMat, mvpMat);
	glUseProgram(this->textureProgram);
	glUniform1i(this->textureProgram_texUnit, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->rect_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*)texcoords);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_STENCIL_TEST);
	*/
}
void primitives::drawTexture(GLuint texID, float x, float y, const float texcoords[8], float w, float h, float rot){

	setColor(1,1,1,1);
	drawRect(x,y,w,h,rot);
	return;
	/*
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0);
	glRotatef(rot, 0, 0, 1);
	glScalef(w, h, 0);
	glUseProgram(this->textureProgram);
	glUniform1i(this->textureProgram_texUnit, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->rect_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*)texcoords);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	*/
}
void primitives::drawLine(float x1,float y1, float x2, float y2){
	return;
	/*
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
	*/
}

#else


#include "opengl/primitives.h"
#include <cmath>
#include <cstring>
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
"attribute vec2 texture_coord;"
"void main(){"
	"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
	"UV=texture_coord;"
"}";
const GLchar *textureFragmentShader =
"uniform sampler2D myTexture;"
"varying vec2 UV;"
"void main(){"
	"gl_FragColor = texture2D(myTexture, UV);"
"}";
int primitives::init(){
	//generate circle vbo
	glGenBuffers(1, &(this->circle_VBO));
	float circle_coords[(CIRCLE_RES + 2) * 2];
	circle_coords[0] = 0;
	circle_coords[1] = 0;
	for (int i = 1; i < CIRCLE_RES + 2; i++){
		circle_coords[2 * i] = (float)cos(360. / CIRCLE_RES  * PIPERDEG *  i);
		circle_coords[2 * i + 1] = (float)sin(360. / CIRCLE_RES  * PIPERDEG *  i);
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->circle_VBO);
	glBufferData(GL_ARRAY_BUFFER, ((CIRCLE_RES + 2) * 2)*sizeof(float), circle_coords, GL_STATIC_DRAW);

	//generate rect vbo
	float rect_coords[] = {
		-.5f, -.5f,
		.5f, -.5f,
		.5f, .5f,
		-.5f, .5f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};
	glGenBuffers(1, &(this->rect_VBO));
	glBindBuffer(GL_ARRAY_BUFFER, this->rect_VBO);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), rect_coords, GL_STATIC_DRAW);

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
	textureProgram_texCoord = glGetAttribLocation(this->textureProgram, "texture_coord");
	return 1;
}

void primitives::deinit(){

}

void primitives::setupViewport(float l, float r, float t, float b){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(l, r, b, t, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
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
	alpha = clamp(a);
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
	glUniform4f(this->circleProgram_Color, red, green, blue, alpha);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->circle_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glLineWidth(lineWidth);
	glDrawArrays(GL_LINE_LOOP, 2, CIRCLE_RES);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
	glPopMatrix();
}
void primitives::drawLine(float x1, float y1, float x2, float y2){
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
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
	glPopMatrix();
}

void primitives::drawStenciledTexture(GLuint texID, draw_square& mask, draw_square& texture, const float texcoords[8], float rot){
	//draw mask

	glPushMatrix();
	glEnable(GL_STENCIL_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	glStencilFunc(GL_NEVER, 1, 0xFF);
	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
	glStencilMask(0xFF);
	glClear(GL_STENCIL_BUFFER_BIT);

	//draw mask.
	glLoadIdentity();
	glTranslatef(mask.x, mask.y, 0);
	glRotatef(rot, 0, 0, 1);
	glScalef(mask.w, mask.h, 0);
	glUseProgram(this->simpleProgram);
	glUniform4f(this->circleProgram_Color, red, green, blue, alpha);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->rect_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableVertexAttribArray(0);
	glUseProgram(0);



	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
	glStencilMask(0x00);
	glStencilFunc(GL_EQUAL, 1, 0xFF);

	//draw texture
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glLoadIdentity();
	glTranslatef(mask.x, mask.y, 0);
	if (mask.w>mask.h)
		glScalef(mask.w * 2, mask.w * 2, 0);
	else
		glScalef(mask.h * 2, mask.h * 2, 0);
	glUseProgram(this->textureProgram);
	glUniform1i(this->textureProgram_texUnit, 0);
	glVertexAttribPointer(this->textureProgram_texCoord, 8, GL_FLOAT, false, 0, texcoords);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->rect_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*)texcoords);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_STENCIL_TEST);
}
void primitives::drawTexture(GLuint texID, float x, float y, const float texcoords[8], float w, float h, float rot){
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0);
	glRotatef(rot, 0, 0, 1);
	glScalef(w, h, 0);
	glUseProgram(this->textureProgram);
	glUniform1i(this->textureProgram_texUnit, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->rect_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*)texcoords);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void primitives::fillRect(float x, float y, float w, float h, float rot){
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0);
	glRotatef(rot, 0, 0, 1);
	glScalef(w, h, 0);
	glUseProgram(this->simpleProgram);
	glUniform4f(this->circleProgram_Color, red, green, blue, alpha);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->rect_VBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
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

#endif