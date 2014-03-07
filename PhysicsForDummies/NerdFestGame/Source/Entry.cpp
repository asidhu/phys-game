
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
#include "Mob.h"
#include "Player.h"
#include "GameObjects.h"
#include "EnemyCannoneer.h"
#include "EnemyRiflesmen.h"
GameEngine engine;

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



class bgimage :public GameObject{
public:
	unsigned int resID;
	bgimage(body* b, unsigned int id) :GameObject(b,true,true){ resID = id; }
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

void renderDebug(PhysEngine* eng, RenderList* list){
	int numCollisions = eng->debug_getNumCollisions();
	contactdetails* dets = eng->debug_getCollisions();
	for (int i = 0; i < numCollisions; i++){
		contactdetails *cd = dets + i;
		RenderItem* itm = list->getItem();
		itm->myType = solidcircle;
		itm->x = cd->b1->position.x + cd->contactPoint[0].x;
		itm->y = cd->b1->position.y + cd->contactPoint[0].y;
		itm->circle.radius = .4f;
		itm->circle.a = itm->circle.g = 1;
		itm->circle.r = itm->circle.b = 0;
		list->addItem(itm);
		itm = list->getItem();
		itm->myType = solidcircle;
		itm->x = cd->b2->position.x + cd->contactPoint[1].x;
		itm->y = cd->b2->position.y + cd->contactPoint[1].y;
		itm->circle.radius = .4f;
		itm->circle.a = itm->circle.g = 1;
		itm->circle.r = itm->circle.b = 0;
		list->addItem(itm);
		itm = list->getItem();
		/*
		itm->myType = hollowsquare;
		itm->x = cd->b2->position.x + cd->contactPoint[1].x;
		itm->y = cd->b2->position.y + cd->contactPoint[1].y;
		itm->circle.radius = .1f;
		itm->circle.a = itm->circle.g = 1;
		itm->circle.r = itm->circle.b = 0;
		list->addItem(itm);
		*/
	}
}

int main(int argc, char* argv[])
{
	GLFWGraphics *graphics = new GLFWGraphics(1366, 700);
	if (!graphics->m_initialized)
		return -1;
	GLFWInput *input = new GLFWInput(graphics->m_window);
	graphics->m_centerX = 0;
	graphics->m_top = 70;
	//graphics->m_top = 1;
	graphics->m_bottom = -1;
	engine.setup(graphics,input);
	input->m_engine = &engine;
	//create random physics object and add as test...
	Actor* arr[] = {
		new bgimage(createBody(engine.getPhysEngine(), 0, 0, 300, 16, 0), 2),
		new bgimage(createBody(engine.getPhysEngine(), -150, 100, 16,600, 0),	2),
		new bgimage(createBody(engine.getPhysEngine(), 150, 100, 16, 600, 0), 2),
		new bgimage(createBody(engine.getPhysEngine(), 0, 400, 300, 16, 0), 2),
		//WALLS


		new bgimage(createBody(engine.getPhysEngine(), 60, 40, 60, 4, 0), 2),
		new bgimage(createBody(engine.getPhysEngine(), 40, 50, 4, 20, 0), 2),
		//WEIRD SHOOING PLATFORM


		new bgimage(createBody(engine.getPhysEngine(), rand() % 50 - 25, 20, 6, 8, 15), 2),
		new bgimage(createBody(engine.getPhysEngine(), rand() % 50 - 25, 20, 15, 5, 15), 2),
		//RANDOM BOXES
	};
	//arr[6]->getBody()->lockRotation();
	//arr[7]->getBody()->lockRotation();
	Scene* scene = new Scene(1);
	for (int i = 0; i < 8; i++){
		scene->addFixture(arr[i], 0);
	}
	engine.setupScene(scene);
	body* b = createBody(engine.getPhysEngine(), 10, 10,8, 2, 1);
	Player *m = new Player(0,b);
	m->m_hp = 10;
	m->m_tex = 1;
	m->engine = &engine;
	graphics->loadImage(1, "./reddiamond.png");
	graphics->loadImage(2, "./orangediamond.png");
	graphics->loadImage(3, "./bluediamond.png");
	engine.player = m;
	b->data = engine.player;
	//b->post_collide = onGroundCheck;
	engine.addActor(engine.player);
	
	for (int i = 0; i < 0; i++){
		EnemyCannoneer* ec = new EnemyCannoneer(createBody(engine.getPhysEngine(), 45+rand()%5, 60, 2, 2, 1));
		ec->m_tex = 3;
		ec->m_hp = 10;
		engine.addActor(ec);
	}
	for (int i = 0; i < 0; i++){
		EnemyRiflesmen* ec = new EnemyRiflesmen(createBody(engine.getPhysEngine(), -60 + 120 * (float)rand() / RAND_MAX, 10, 2, 2, 1));
		ec->m_tex = 3;
		ec->m_hp = 10;
		engine.addActor(ec);
	}
	//b->angularDamping = .9998f;
	//b->lockRotation();
	engine.getPhysEngine()->enableDebugger(false);
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