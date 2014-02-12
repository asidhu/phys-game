
#include <GLFW\glfw3.h>
#include "opengl\opengl.h"
#include "opengl\primitives.h"
#include "opengl\stb_image.h"
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <time.h>
#include <PhysicsEngine\Source\PhysEngine.h>
#include "GameEngine.h"
#include "Actor.h"
primitives renderer;
GLuint texID;
GameEngine engine;
GLFWwindow* window;

#define random() (float)rand()/RAND_MAX
#define rad2deg(x) (x*57.2957795131) 
bool step = false, go = true;
void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void mouse_callback(GLFWwindow* win, int button, int action, int mods){
	double x, y; 
	glfwGetCursorPos(win, &x, &y);
	engine.handleMouse((int)floor(x),(int)floor(y),button,action);
}
static void mousepos_callback(GLFWwindow* win, double x, double y){
	engine.handleMouseMove((int)floor(x), (int)floor(y));
}	
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	engine.handleKey(key, action);
	/*
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		step = true;
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
		go = !go;
	if (key == GLFW_KEY_SPACE && action == GLFW_REPEAT)
		step = true;
		*/


}
void render(PhysEngine* phys,GLFWwindow* window){
	float ratio;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float)height;
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10*ratio, 10*ratio, -1.f, 19.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	std::vector<body*> bodies = phys->getBodies();
	std::vector<body*>::iterator it = bodies.begin();
	renderer.setColor(1, 1, 1, 1);
	renderer.setLineWidth(1);
	while (it != bodies.end()){
		body* b = *it;
		if (b->form->type == CIRCLE_TYPE)
			renderer.drawCircle(b->position.x, b->position.y, ((circle*)b->form)->radius);
		else if (b->form->type == BOX_TYPE)
		{
			renderer.drawRect(b->position.x, b->position.y, ((box*)b->form)->halfwidth * 2, ((box*)b->form)->halfheight * 2, rad2deg(b->rotation));
			renderer.drawLine(b->position.x, b->position.y, b->position.x + cos(b->rotation)*.1f, b->position.y + .1f*sin(b->rotation));
			//renderer.drawRect(b->position.x, b->position.y, ((box*)b->form)->halfwidth * 2, ((box*)b->form)->halfheight * 2, 0);
		}
		it++;
	}
	glfwSwapBuffers(window);
}

body* createBody(PhysEngine* engine, float x, float y, float w, float h, float mass, float rot = 0){
	bodydef bdef;
	bdef.position.x = x;
	bdef.position.y = y;
	bdef.width = w;
	bdef.height = h;
	bdef.mass = mass;
	bdef.rotation = rot;
	return engine->buildBody(bdef);
}


void onGroundCheck(body* b, contactdetails* dets){
	if (((Actor*)b->data)->onGround || dets->contactNormal.dot(vec2(0, 1)) < .5f)
		return;
	float relV = b->velocity.dot(vec2(0,1));
	if (abs(relV) < 1)
		((Actor*)b->data)->onGround = true;
}

int main(int argc, char* argv[])
{
	//setup error callback
	glfwSetErrorCallback(error_callback);
	//initiate GLFW
	if (!glfwInit())
		return -1;
	int x, y, comp;
	//create a window
	window = glfwCreateWindow(800, 600, "GLFW", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	GLenum err = glewInit();
	if (err != GLEW_OK){
		std::cout << "GLEW ERROR" << std::endl;
	}
	std::cout << "GLEW Version " << glewGetString(GLEW_VERSION)<<std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

	unsigned char* data = stbi_load("./test.bmp", &x, &y, &comp, 0);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, (comp == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	renderer.init();
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mousepos_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	float ratio;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float)height;


	//create random physics object and add as test...
	createBody(engine.getPhysEngine(), 0, 0, 50, 1, 0);
	createBody(engine.getPhysEngine(), -25, 25, 1, 50, 0);
	createBody(engine.getPhysEngine(), 25, 25, 1, 50, 0);
	createBody(engine.getPhysEngine(), 25, 9, 1, 50, 0,2);
	body* b=createBody(engine.getPhysEngine(), 5, 5, 1, 1, 1);
	engine.player = new Actor(0, b);
	b->data = engine.player;
	b->on_collide = onGroundCheck;
	engine.addActor(engine.player);
	b->angularDamping = .7f;
	//b->lockRotation();


	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	int frames = 0;
	double totTime = 0;
	while (!glfwWindowShouldClose(window))
	{
		LARGE_INTEGER start, stop;

		render(engine.getPhysEngine(),window);

		Sleep(10);

		engine.tick();
		glfwPollEvents();
		if (step || go){
			QueryPerformanceCounter(&start);
			QueryPerformanceCounter(&stop);

			stop.QuadPart = stop.QuadPart - start.QuadPart;
			double time = (double)stop.QuadPart / freq.QuadPart * 1000;
			totTime += time;
			if (frames++ > 100){
				std::cout << "time:" << totTime / frames << std::endl;
				frames = 0;
				totTime = 0;

			}
			step = false;
		}
		
		// Keep running
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

