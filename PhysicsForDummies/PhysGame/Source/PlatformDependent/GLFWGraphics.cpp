#include "PhysGame\Source\PlatformDependent\GLFWGraphics.h"
#define GLEW_STATIC
#include <GLFW\glfw3.h>
#include "opengl\opengl.h"
#include "opengl\primitives.h"
#include "opengl\stb_image.h"

static void error_callback(int error, const char* description)
{
	//fputs(description, stderr);
}
GLFWGraphics::GLFWGraphics(int w, int h){
	m_initialized = false;
	//setup error callback
	glfwSetErrorCallback(error_callback);
	//initiate GLFW
	if (!glfwInit()){
		return;
	}
	int x, y, comp;
	//create a window
	m_window = glfwCreateWindow(w, h, "GLFW", NULL, NULL);
	glfwMakeContextCurrent(m_window);
	GLenum err = glewInit();
	if (err != GLEW_OK){
		return;
	}

}