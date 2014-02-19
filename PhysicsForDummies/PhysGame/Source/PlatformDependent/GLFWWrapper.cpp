#ifdef EMSCRIPTEN
#include "GLFWWrapper.h"

GLFW3mousebuttonfun current_mousebtn_func = NULL;
GLFW3keyfun current_key_func = NULL;
GLFW3cursorposfun current_mousepos_func = NULL;
void glfwSetErrorCallback(GLFW3errorfun){}


GLFWwindow* glfwCreateWindow(int w, int h, const char* title, int, int){
	glfwOpenWindow(w, h, 5, 6, 5, 0, 0, 0, GLFW_WINDOW);
	return NULL;
}

void glfwMakeContextCurrent(GLFWwindow*){}

void glfwGetWindowSize(GLFWwindow*, int* w, int *h){
	glfwGetWindowSize(w, h);
}

void glfwGetFramebufferSize(GLFWwindow*, int* w, int *h){
	glfwGetWindowSize(w, h);
}

void glfwSwapBuffers(GLFWwindow*){
	glfwSwapBuffers();
}

void keycallback(int key, int action){
	if (current_key_func != NULL)
		current_key_func(NULL, key, NULL, action, NULL);
}
void glfwSetKeyCallback(GLFWwindow*, GLFW3keyfun a){
	current_key_func = a;
	glfwSetKeyCallback(keycallback);
}

void mouseposcallback(int x, int y){
	if (current_mousepos_func != NULL)
		current_mousepos_func(NULL, (double)x, (double)y);
}

void glfwSetCursorPosCallback(GLFWwindow*, GLFW3cursorposfun func){
	current_mousepos_func = func;
	glfwSetMousePosCallback(mouseposcallback);
}

void mousebuttoncallback(int key, int action){
	if(key ==2)
		key = GLFW_MOUSE_BUTTON_RIGHT;
	if (current_mousebtn_func != NULL)
		current_mousebtn_func(NULL, key, action, NULL);
}

void glfwSetMouseButtonCallback(GLFWwindow*, GLFW3mousebuttonfun func){
	current_mousebtn_func = func;
	glfwSetMouseButtonCallback(mousebuttoncallback);
}

bool glfwWindowShouldClose(GLFWwindow*){ return false; }

void glfwGetCursorPos(GLFWwindow*, double* x, double* y){
	int tX, tY;
	glfwGetMousePos(&tX, &tY);
	*x = (double)tX;
	*y = (double)tY;
}


#endif