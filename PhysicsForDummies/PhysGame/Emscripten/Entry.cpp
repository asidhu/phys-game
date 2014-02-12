#define GLEW_STATIC
#include "opengl\primitives.h"
#include <stdio.h>
#include <stdlib.h>
#include <PhysicsEngine\Source\PhysEngine.h>
#include <GL/glfw.h>
#include <emscripten/emscripten.h>
primitives renderer;
PhysEngine phys;
GLuint texID;
#define random() (float)rand()/RAND_MAX
void render();
void iteration();

int main(int argc, char* argv[])
{
	phys=PhysEngine();
	float ratio;
	int width = 320, height = 240;
	//initiate GLFW
	if (!glfwInit())
		return -1;
	int x, y, comp;
	//create a window
	if (glfwOpenWindow(width, height, 5, 6, 5,
		0, 0, 0, GLFW_WINDOW) != GL_TRUE)
		return -1;
	glfwGetWindowSize(&width, &height);
	ratio = width / (float)height;
	if (!renderer.init(-ratio,ratio,-1,1))
		return -1;
	//ratio = 1;
	phys.setup(0, -ratio, -1, ratio, 1);
	for (int i = 0; i < 20; i++){
		phys.createBody(random() * 2 * ratio - ratio, random() * 2 - 1, .05f,random()*.2f-.1f,random()*.2f-.1f);
	}
	emscripten_set_main_loop(iteration,60,1);
	glfwTerminate();
	return 0;
}

void iteration(){
	render();
	phys.step(1.f / 32.f);
	glfwPollEvents();
	// Keep running
}
void render(){
	float ratio;
	int width, height;
	glfwGetWindowSize(&width, &height);
	ratio = width / (float)height;
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	std::vector<body*> bodies = phys.getBodies();
	std::vector<body*>::iterator it = bodies.begin();
	renderer.setColor(1, 1, 1, 1);
	renderer.setLineWidth(3);
	while (it != bodies.end()){
		body* b = *it;
		renderer.drawCircle(b->x, b->y, b->radius);
		it++;
	}
	glfwSwapBuffers();
}