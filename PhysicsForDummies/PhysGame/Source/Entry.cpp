
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
#include "NerdFestGame\Mob.h"
#include "NerdFestGame\Player.h"
#include "NerdFestGame\GameObjects.h"
#include "NerdFestGame\EnemyCannoneer.h"
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


int main(int argc, char* argv[])
{
	GLFWGraphics *graphics = new GLFWGraphics(1024, 800);
	if (!graphics->m_initialized)
		return -1;
	GLFWInput *input = new GLFWInput(graphics->m_window);
	graphics->m_centerX = 0;
	graphics->m_top = 80;
	//graphics->m_top = 1;
	graphics->m_bottom = -1;
	engine.setup(graphics,input);
	input->m_engine = &engine;
	//create random physics object and add as test...
	Actor* arr[] = {
		new bgimage(createBody(engine.getPhysEngine(), 0, 0, 120, 4, 0),		2),
		new bgimage(createBody(engine.getPhysEngine(), -60, 40, 4, 85, 0),	2),
		new bgimage(createBody(engine.getPhysEngine(), 60, 40, 4, 85, 0),	2)
	};
	Scene* scene = new Scene(1);
	for (int i = 0; i < 3; i++){
		scene->addFixture(arr[i], 0);
	}
	engine.setupScene(scene);
	body* b = createBody(engine.getPhysEngine(), 10, 10,2, 2, 1);
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
	for (int i = 0; i < 5; i++){
		EnemyCannoneer* ec = new EnemyCannoneer(createBody(engine.getPhysEngine(), -60+120*(float)rand()/RAND_MAX, 10, 2, 2, 1));
		ec->m_tex = 3;
		engine.addActor(ec);
	}
	//b->angularDamping = .9998f;
	//b->lockRotation();


	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	int frames = 0;
	double totTime = 0;
	while (!engine.shouldClose())
	{
		LARGE_INTEGER start, stop;


		if (step || go){

			QueryPerformanceCounter(&start);
			engine.tick();
			engine.render();
			QueryPerformanceCounter(&stop);

			stop.QuadPart = stop.QuadPart - start.QuadPart;
			double time = (double)stop.QuadPart / freq.QuadPart * 1000;
			const float waittime = 1000.f / 60.f;
			if (time<waittime)Sleep(waittime - time);
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

