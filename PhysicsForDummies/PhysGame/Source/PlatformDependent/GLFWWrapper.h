#pragma once

#ifdef EMSCRIPTEN
#include <gl\glfw.h>
struct GLFWwindow;
typedef void (*GLFWkeyfun)(GLFWwindow*,int,int,int,int);
typedef void (*GLFWmousebuttonfun)(GLFWwindow*,int,int,int);
typedef void (*GLFWcursorposfun)(GLFWwindow*,double, double);
typedef void (*GLFWerrorfun)(int, const char*);


GLFWmousebuttonfun current_mousebtn_func = NULL;
GLFWkeyfun current_key_func=NULL;
GLFWcursorposfun current_mousepos_func=NULL;
void glfwSetErrorCallback(GLFWerrorfun){}


void glfwCreateWindow(GLFWwindow*, int w, int h, const char* title, int,int){
	glfwOpenWindow(w, h, 5, 6, 5,0, 0, 0, GLFW_WINDOW);
}

void glfwMakeContextCurrent(GLFWwindow*){}

void glfwGetWindowSize(GLFWwindow*, int* w, int *h){
	glfwGetWindowSize(w,h);
}

void glfwGetFramebufferSize(GLFWwindow*, int* w, int *h){
	glfwGetWindowSize(w,h);
}

void glfwSwapBuffers(GLFWwindow*){
	glfwSwapBuffers();
}

void keycallback(int key, int action){
	if(current_key_func!=NULL)
		current_key_func(NULL,key,NULL,action,NULL);
}
void glfwSetKeyCallback(GLFWkeyfun a){
	current_key_func = a;
	glfwSetKeyCallback(keycallback);
}

void mouseposcallback(int x, int y){
	if(current_mousepos_func!=NULL)
		current_mousepos_func(NULL, (double) x, (double) y);
}

void glfwSetCursorPosCallback(GLFWcursorposfun func){
	current_mousepos_func=func;
	glfwSetMousePosCallback(mouseposcallback);
}

void mousebuttoncallback(int key, int action){
	if (current_mousebtn_func != NULL)
		current_mousebtn_func(NULL, key,action,NULL);
}

void glfwSetMouseButtonCallback(GLFWcursorposfun func){
	current_mousebtn_func = func;
	glfwSetMouseButtonCallback(mousebuttoncallback);
}

void glfwWindowShouldClose(GLFWwindow*){}

void glfwGetCursorPos(GLFWwindow*, int* x, int* y){
	glfwGetMousePos(x,y);
}


#else

#include <GLFW\glfw3.h>


#endif