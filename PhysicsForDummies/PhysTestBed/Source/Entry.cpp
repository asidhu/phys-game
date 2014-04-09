
#include <stdio.h>
#include <iostream>
#ifndef EMSCRIPTEN
#include <Windows.h>
#else
#include <emscripten/emscripten.h>
#endif

#include <time.h>
#include <PhysicsEngine\Source\PhysEngine.h>
#include "PhysGame\Source\PlatformDependent\GLFWGraphics.h"
#include "PhysGame\Source\PlatformDependent\GLFWInput.h"
#include "PhysGame\Source\Scene.h"
#include "PhysGame\Source\GameEngine.h"
#include "PhysGame\Source\Actor.h"
#include "PhysGame\Source\GameInputHandler.h"
GameEngine engine;
PhysEngine* physng;

#define random() (float)rand()/RAND_MAX
#define rad2deg(x) (x*57.2957795131) 
bool step = false, go = true;
void iteration();

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

class handler : public GameInputHandler{
	body* selected;
	vec2 loc;
public:
	handler(){
		selected = NULL;
	}
	void tick(){
		if (selected != NULL){
			vec2 dist = (loc - selected->position);
			float dlen = dist.lengthSq();
			dist.normalize();
			selected->impulse += dist * 5.f*(1-1/(dlen));
		}
	}
	virtual void handleKey(int key, int state){
		physng->debug_collisionAdvance();
		
	}
	virtual void handleMouse(float x, float y, int button, int state){
		if (selected != NULL)
		{
			selected = NULL;
			return;
		}
		for (body* ptr = physng->getBodyList(); ptr != 0; ptr = ptr->nextX){
			if (ptr->AABB.contains(x, y))
			{
				selected = ptr;
				break;
			}
		}
	}
	virtual void handleMouseMove(float x, float y){
		loc.x = x;
		loc.y = y;
	}
};



class bgimage :public Actor{
public:
	unsigned int resID;
	bgimage(body* b, unsigned int id) :Actor(1,b){ resID = id; }
	void render(RenderList* lst){
		RenderItem* item = lst->getItem();
		//FIX THIS USE ACCESSOR :OOOO
		body* b = getBody();
		item->x = b->position.x;
		item->y = b->position.y;
		item->myType = texture;
		item->tex.w = ((box*)b->form)->halfwidth * 2;
		item->tex.h = ((box*)b->form)->halfheight * 2;
		item->tex.s1 = item->y*.4f;
		item->tex.t1 = item->x*.4f;
		item->tex.s2 = item->y*.4f - item->tex.h;
		item->tex.t2 = item->x*.4f - item->tex.w;
		item->zIndex = .1f;
		//item->tex.s1 = item->tex.t1 = 0;
		//item->tex.s2 = item->tex.t2 = 1;
		item->tex.resID = resID;
		item->rot = b->rotation;
		lst->addItem(item);

		item = lst->getItem();
		item->x = b->position.x;
		item->y = b->position.y;
		item->myType = hollowsquare;
		item->rot = b->rotation;
		item->square.lw = 1;
		item->square.w = ((box*)b->form)->halfwidth * 2;
		item->square.h = ((box*)b->form)->halfheight * 2;
		item->square.a = 1;
		item->square.r = item->square.g = item->square.b = 1;
		lst->addItem(item);
	}
};

void createBoxs(GameEngine* eng, PhysEngine* phys){
	Scene* scene = new Scene(1);
	engine.setupScene(scene);

	engine.addActor(new bgimage(createBody(engine.getPhysEngine(), -5, 5, 10, 3, 0, -45.f / 180.f * 3.14159f), 2));
	engine.addActor(new bgimage(createBody(engine.getPhysEngine(), 5, 5, 10, 3, 0, 45.f / 180.f * 3.14159f), 2));
	for (int i = 0; i < 1; i++)
		engine.addActor(new bgimage(createBody(engine.getPhysEngine(), rand() % 6 - 3, rand() %50 + 5, 1, 1, 1), 2));
	engine.getPhysEngine()->enableDebugger(false);
}



void renderDebug(PhysEngine* eng, RenderList* list){
	int numCollisions = eng->debug_getNumCollisions();
	contactdetails* dets = eng->debug_getCollisions();
	for (int i = 0; i < numCollisions; i++){
		contactdetails *cd = dets + i;
		RenderItem* itm = list->getItem();
		itm->myType = solidcircle;
		itm->x = cd->b1->position.x + cd->contactPoint[0].x;
		itm->y = cd->b1->position.y + cd->contactPoint[0].y;
		itm->circle.radius = .1f;
		itm->circle.a = itm->circle.g = 1;
		itm->circle.r = itm->circle.b = 0;
		list->addItem(itm);
		itm = list->getItem();
		itm->myType = solidcircle;
		itm->x = cd->b2->position.x + cd->contactPoint[1].x;
		itm->y = cd->b2->position.y + cd->contactPoint[1].y;
		itm->circle.radius = .1f;
		itm->circle.a = itm->circle.g = 1;
		itm->circle.r = itm->circle.b = 0;
		list->addItem(itm);
		itm = list->getItem();
		
		itm->myType = hollowsquare;
		itm->rot = atan2(cd->contactNormal.y, cd->contactNormal.x);
		itm->x = cd->b1->position.x+cd->contactPoint[0].x+.5f*cos(itm->rot);
		itm->y = cd->b1->position.y + cd->contactPoint[0].y+.5f*sin(itm->rot);
		itm->square.w = 1;
		itm->square.h = .1;
		itm->square.a = itm->square.g = 1;
		itm->square.r = itm->square.b = 0;
		list->addItem(itm);
		
	}
}

int main(int argc, char* argv[])
{
	handler h;
	GLFWGraphics *graphics = new GLFWGraphics(1366, 700);
	if (!graphics->m_initialized)
		return -1;
	GLFWInput *input = new GLFWInput(graphics->m_window);
	graphics->m_centerX = 0;
	graphics->m_top = 20;
	graphics->m_bottom = -10;
	engine.setup(graphics,input,&h);
	physng = engine.getPhysEngine();
	input->m_engine = &engine;
	/*
	Scene* scene = new Scene(1);
	engine.setupScene(scene);
	engine.addActor(new bgimage(createBody(engine.getPhysEngine(), 0, 0, 15, 3, 0, -45.f / 180.f * 3.14159f), 2));
	engine.addActor(new bgimage(createBody(engine.getPhysEngine(), -6, 10, 3, 3, 1,-45.f/180.f*3.14159f), 2));
	engine.getPhysEngine()->enableDebugger(false);
	*/
	createBoxs(&engine, physng);
#ifndef EMSCRIPTEN
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	int frames = 0;
	double totTime = 0;
	while (!engine.shouldClose())
	{
		LARGE_INTEGER start, stop;


		if (step || go){

			renderDebug(engine.getPhysEngine(), engine.getDebugList());


			QueryPerformanceCounter(&start);
			engine.render();
			QueryPerformanceCounter(&stop);

			stop.QuadPart = stop.QuadPart - start.QuadPart;
			double render= (double)stop.QuadPart / freq.QuadPart * 1000;

			QueryPerformanceCounter(&start);
			engine.tick();
			QueryPerformanceCounter(&stop);

			h.tick();

			stop.QuadPart = stop.QuadPart - start.QuadPart;
			double tick = (double)stop.QuadPart / freq.QuadPart * 1000;
			double time = render + tick;
			const float waittime = 1000.f / 60.f;
			if (time<waittime)Sleep((DWORD)(waittime - time));
			totTime += time;
			if (frames++ > 100){
				std::cout << "time:" << totTime / frames << " tick:" << tick << " render:" << render;
				std::cout << " actors:" << engine.getNumActors();
				std::cout << " effects:" << engine.getNumEffects();
				std::cout << " bodies:" << engine.getPhysEngine()->getNumBodies() << std::endl;
				frames = 0;
				totTime = 0;

			}
			step = false;
		}
		// Keep running
	}
#else
	emscripten_set_main_loop(iteration,60,1);
#endif
	return 0;
}

#ifdef EMSCRIPTEN
void iteration(){
	engine.render();
	engine.tick();
}
#else
void iteration(){}

#endif