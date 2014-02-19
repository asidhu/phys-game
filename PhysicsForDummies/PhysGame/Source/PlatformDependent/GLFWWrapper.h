#pragma once

#ifdef EMSCRIPTEN
#include <gl\glfw.h>
struct GLFWwindow;
typedef void (*GLFW3keyfun)(GLFWwindow*,int,int,int,int);
typedef void (*GLFW3mousebuttonfun)(GLFWwindow*,int,int,int);
typedef void (*GLFW3cursorposfun)(GLFWwindow*,double, double);
typedef void (*GLFW3errorfun)(int, const char*);

void glfwSetErrorCallback(GLFW3errorfun);


GLFWwindow* glfwCreateWindow(int w, int h, const char* title, int,int);

void glfwMakeContextCurrent(GLFWwindow*);

void glfwGetWindowSize(GLFWwindow*, int* w, int *h);

void glfwGetFramebufferSize(GLFWwindow*, int* w, int *h);

void glfwSwapBuffers(GLFWwindow*);

void keycallback(int key, int action);
void glfwSetKeyCallback(GLFWwindow*,GLFW3keyfun a);

void mouseposcallback(int x, int y);
void glfwSetCursorPosCallback(GLFWwindow*,GLFW3cursorposfun func);

void mousebuttoncallback(int key, int action);

void glfwSetMouseButtonCallback(GLFWwindow*,GLFW3mousebuttonfun func);

bool glfwWindowShouldClose(GLFWwindow*);

void glfwGetCursorPos(GLFWwindow*, double* x, double* y);


#else

#include <GLFW\glfw3.h>


#endif