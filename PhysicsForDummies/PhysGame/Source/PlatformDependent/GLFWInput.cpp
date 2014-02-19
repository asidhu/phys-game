#include "PhysGame\Source\PlatformDependent\GLFWInput.h"
#include "GLFWWrapper.h"
#include "PhysGame\Source\GameEngine.h"
#include "PhysGame\Source\PlatformDependent\GLFWGraphics.h"
#include <iostream>
GLFWInput *current_input;
static void mouse_callback(GLFWwindow* win, int button, int action, int mods){
	double x, y;
	glfwGetCursorPos(win, &x, &y);
	GLFWGraphics* gfx = (GLFWGraphics*)current_input->getGraphics();
	y = gfx->m_win_height - y;
	float worldX = ((float)x / gfx->m_win_width*(gfx->getWidth()) + gfx->getLeft());
	float worldY = ((float)y / gfx->m_win_height*(gfx->m_top - gfx->m_bottom) + gfx->m_bottom);
	int out = 0, outbtn = 0;
	if (action == GLFW_PRESS)
		out = MOUSE_PRESS;
	else
		out = MOUSE_RELEASE;
	if (button == GLFW_MOUSE_BUTTON_LEFT){
		outbtn = MOUSE_LEFT;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT){
		outbtn = MOUSE_RIGHT;
	}
	current_input->m_engine->handleMouse(worldX, worldY, outbtn, out);
}
static void mousepos_callback(GLFWwindow* win, double x, double y){
	GLFWGraphics* gfx = (GLFWGraphics*)current_input->getGraphics();
	float worldX = ((float)x / gfx->m_win_width*(gfx->getWidth()) + gfx->getLeft());
	float worldY = ((float)y / gfx->m_win_height*(gfx->m_top - gfx->m_bottom) + gfx->m_top);
	current_input->m_engine->handleMouseMove(worldX, worldY);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	int out = 0;
	if (action == GLFW_PRESS)
		out = KEY_DOWN;
	else if (action == GLFW_RELEASE)
		out = KEY_UP;
	if(out!=0)
		current_input->m_engine->handleKey(key, out);
	/*
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	step = true;
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	go = !go;
	if (key == GLFW_KEY_SPACE && action == GLFW_REPEAT)
	step = true;
	*/
}

GLFWInput::GLFWInput(GLFWwindow* win){
	m_window = win;
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetCursorPosCallback(m_window, mousepos_callback);
	glfwSetMouseButtonCallback(m_window, mouse_callback);
}
void GLFWInput::pollInput(){
	current_input = this;
	glfwPollEvents();
	if (glfwWindowShouldClose(m_window))
		m_engine->close();
}