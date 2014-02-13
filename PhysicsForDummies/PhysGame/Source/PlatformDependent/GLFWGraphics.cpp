#include "PhysGame\Source\PlatformDependent\GLFWGraphics.h"
#define GLEW_STATIC
#include "opengl\opengl.h"
#include "opengl\primitives.h"
#include "opengl\stb_image.h"
#include "PhysGame\Source\RenderList.h"
#define rad2deg(x) (x*57.2957795131f) 
primitives renderer;

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
	//create a window
	m_window = glfwCreateWindow(w, h, "GLFW", NULL, NULL);
	glfwMakeContextCurrent(m_window);
	GLenum err = glewInit();
	if (err != GLEW_OK){
		return;
	}
	renderer.init();
	m_initialized = true;
	m_win_width = w;
	m_win_height = h;
}
void GLFWGraphics::start(){
	float  ratio;
	int width, height;
	float viewHeight = m_top - m_bottom;
	glfwGetFramebufferSize(m_window, &width, &height);
	glfwGetWindowSize(m_window, &m_win_width, &m_win_height);
	ratio = width / (float)height;
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(m_left, m_right, m_bottom, m_top, 1.f, -1.f);
	m_left = m_centerX - viewHeight / 2 * ratio;
	m_right = m_centerX + viewHeight / 2 * ratio;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void GLFWGraphics::close(){
	glfwSwapBuffers(m_window);
}
void GLFWGraphics::drawList(RenderList* list){
	renderer.setColor(1, 1, 1, 1);
	renderer.setLineWidth(1);
	for (std::list<RenderItem*>::iterator it = list->renderItems.begin(); it != list->renderItems.end(); it++){
		RenderItem* item = *it;
		renderer.drawRect(item->x, item->y, item->w, item->h, rad2deg(item->rot));
	}
}