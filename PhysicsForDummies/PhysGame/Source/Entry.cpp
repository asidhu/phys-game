
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <time.h>
#include <PhysicsEngine\Source\PhysEngine.h>
#include "PhysGame\Source\PlatformDependent\GLFWGraphics.h"
#include "PhysGame\Source\PlatformDependent\GLFWInput.h"
#include "PhysGame\Source\StaticFixture.h"
#include "PhysGame\Source\Scene.h"
#include "GameEngine.h"
#include "Actor.h"
GameEngine engine;

#define random() (float)rand()/RAND_MAX
#define rad2deg(x) (x*57.2957795131) 
bool step = false, go = true;

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
	GLFWGraphics *graphics = new GLFWGraphics(1024, 800);
	if (!graphics->m_initialized)
		return -1;
	GLFWInput *input = new GLFWInput(graphics->m_window);
	graphics->m_centerX = 0;
	graphics->m_top = 50;
	graphics->m_bottom = -1;
	engine.setup(graphics,input);
	input->m_engine = &engine;
	//create random physics object and add as test...
	StaticFixture* arr[] = { new StaticFixture(createBody(engine.getPhysEngine(), 0, 0, 50, 1, 0)),
		new StaticFixture(createBody(engine.getPhysEngine(), -22, 25, 1, 50, 0)),
		new StaticFixture(createBody(engine.getPhysEngine(), 22, 25, 1, 50, 0)),
		new StaticFixture(createBody(engine.getPhysEngine(), 25, 9, 1, 50, 0, 2))
	};
	Scene* scene = new Scene(1);
	for (int i = 0; i < 4; i++)
		scene->addFixture(arr[i], 0);
	engine.setupScene(scene);
	body* b = createBody(engine.getPhysEngine(), 5, 5, 1, 1, 1);
	engine.player = new Actor(0, b);
	b->data = engine.player;
	b->on_collide = onGroundCheck;
	engine.addActor(engine.player);
	b->angularDamping = 1;
	//b->angularDamping = .9998f;
	//b->lockRotation();


	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	int frames = 0;
	double totTime = 0;
	while (!engine.shouldClose())
	{
		LARGE_INTEGER start, stop;

		Sleep(10);

		engine.tick();
		engine.render();
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
	return 0;
}

