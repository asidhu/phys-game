#include "PhysGame\Source\PlatformDependent\GLFWInput.h"
#include <GLFW\glfw3.h>


GLFWInput *current_input;
static void mouse_callback(GLFWwindow* win, int button, int action, int mods){
	double x, y;
	glfwGetCursorPos(win, &x, &y);
	int out = 0, outbtn = 0;
	if (action == GLFW_PRESS)
		out = MOUSE_PRESS;
	else
		out = MOUSE_RELEASE;
	if (button == GLFW_MOUSE_BUTTON_LEFT)
		outbtn = MOUSE_LEFT;
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		outbtn = MOUSE_RIGHT;
	if (outbtn != 0 && current_input->onMouse != NULL){
		current_input->onMouse((int)(x), (int)(y), outbtn, out);
	}
}
static void mousepos_callback(GLFWwindow* win, double x, double y){
	if (current_input->onMouseMove != NULL){
		current_input->onMouseMove((int)(x), (int)(y));
	}
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (current_input->onKey != NULL){
		current_input->onKey(key, action);
	}
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
}