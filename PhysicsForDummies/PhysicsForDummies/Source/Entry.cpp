#define GLEW_STATIC
#include "opengl\opengl.h"
#include "opengl\primitives.h"
#include <stdio.h>
#include "opengl\stb_image.h"
#include <iostream>
#include <Windows.h>
#include <time.h>
#include <PhysicsEngine\Source\PhysEngine.h>
primitives renderer;
PhysEngine phys;
GLuint texID;
#define random() (float)rand()/RAND_MAX
#define rad2deg(x) (x*57.2957795131f) 
bool step = false, go = true;
void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		step = true;
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
		go = !go;
	if (key == GLFW_KEY_SPACE && action == GLFW_REPEAT)
		step = true;
}
void render(GLFWwindow* window){
	float ratio;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float)height;
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//std::vector<body*> bodies = phys.getBodies();
	//std::vector<body*>::iterator it = bodies.begin();
	//renderer.setColor(1, 1, 1, 1);
	//renderer.setLineWidth(1);
	renderer.drawTexture(texID, 0,0);
	/*while (it != bodies.end()){
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
	}*/
	glfwSwapBuffers(window);
}
int main(int argc, char* argv[])
{
	phys=PhysEngine();
	//setup error callback
	glfwSetErrorCallback(error_callback);
	//initiate GLFW
	if (!glfwInit())
		return -1;
	int x, y, comp;
	//create a window
	GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW", NULL, NULL);
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

	unsigned char* data = stbi_load("./reddiamond.png", &x, &y, &comp, 0);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, (comp == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	renderer.init();
	glfwSetKeyCallback(window, key_callback);
	float ratio;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float)height;
	phys.setup(-1.f, -ratio, -1, ratio, 1);
	phys.createBox(0, -1, 5 * ratio, .5f, 0, 0, 0, 0, 0);
	phys.createBox(-3*ratio, -0,  20*ratio, .5f, 0, 0, 0, 0, -1.5);
	phys.createBox(3*ratio, -0, 20*ratio, .5f, 0, 0, 0, 0, 1.5);


	//phys.createBox(0.000, 1, .4f, .08f, 0, 0, 1, .0f, 0);
	//phys.createBox(.0f, 1.5f, .2f, .08f, 0, 0, 1, .0f, 0);
	//phys.createBox(.0f, 1.7f, .4f, .08f, 0, 0, 1, .0f, 0);
	//phys.createBox(.0f, 1.9f, .2f, .08f, 0, 0, 1, .0f, 0);
	//phys.createBox(.000f, 5.f, .1f, .08f, 0, 0, 1.1, .0f, 0);

	for (int i = 0; i < 18; i++){
		float mass = random()*.02f + .005f;
		if (mass<.015f){
			//mass = .005f;
		}
		mass = .2f;
		//phys.createBox(random()*1-.5f, 1.5f, .1f, .1f, 0, 0, 1, .4f, random());
		//phys.createBox(0, 2 + 3.f*i, mass + i*.1f, mass, 0, 0, 2, .0f, 0);
		
		if (random()>1.f)
			phys.createBody(random() * 2.f * ratio - ratio, 5.f, mass ,random()*.8f-.4f,random()*.8f-.4f,mass,.9f);
		else
			phys.createBox(random() * 4.f * ratio - ratio, 5.f+ random()*4.f, mass, mass, 0.f, 0.f, 2.f+(float)i, .0f, ((i%2)==1) ? 0.78539816339f : 0.1f);
			
	}
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	int frames = 0;
	double totTime = 0;
	while (!glfwWindowShouldClose(window))
	{
		LARGE_INTEGER start, stop;

		render(window);

		Sleep(10);

		glfwPollEvents();
		if (step || go){
			QueryPerformanceCounter(&start);
			phys.step(1.f / 32.f);
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

