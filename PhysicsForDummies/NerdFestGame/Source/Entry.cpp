
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
#include "PhysGame\Source\SceneManager.h"
#include "PhysGame\Source\PlatformDependent\GLFWInput.h"
#include "PhysGame\Source\Scene.h"
#include "PhysGame\Source\GameEngine.h"
#include "PhysGame\Source\Actor.h"
#include "PhysGame\Source\LayerTexture.h"
#include "PhysGame\Source\GameWorld.h"
#include "PhysGame\Source\LayerWorldViewport.h"
#include "PhysGame\Source\LayerButton.h"
#include "Mob.h"
#include "Player.h"
#include "GameObjects.h"
#include "EnemyRiflesmen.h"
#include "PhysGame\Source\ResourceManager.h"
#include "MyInputHandler.h"
#include "PhysGame\Source\LuaEngine.h"
#include "MyActorManager.h"
#include "LuaBindings.h"
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

int main(int argc, char* argv[])
{
	GLFWGraphics *graphics = new GLFWGraphics(1366, 700);
	if (!graphics->m_initialized)
		return -1;
	GLFWInput *input = new GLFWInput(graphics->m_window);
	MyEngine gamespecific = MyEngine();
	gamespecific.game_engine = &engine;
	MyInputHandler *handler = new MyInputHandler(&gamespecific);
	MyActorManager *actor_manager = new MyActorManager();
	engine.setup(graphics,input,handler,actor_manager);
	actor_manager->m_game_engine = &engine;
	actor_manager->m_my_engine = &gamespecific;
	input->m_engine = &engine;
	engine.getSceneManager()->getLuaEngine()->m_hook = registerGameBindings; // register hook to lua binding function
	engine.start("scenes.dat"); //loads game specific scenes
#ifndef EMSCRIPTEN
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	int frames = 0;
	double totTime = 0;
	while (!engine.shouldClose())
	{
		LARGE_INTEGER start, stop;


		if (step || go){

			//renderDebug(engine.getPhysEngine(), engine.getDebugList());


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
				std::cout << "time:" << totTime / frames << " tick:" << tick << " render:" << render<<std::endl;
				//std::cout << " actors:" << engine.getNumActors();
				//std::cout << " effects:" << engine.getNumEffects();
				//std::cout << " bodies:" << engine.getPhysEngine()->getNumBodies() << std::endl;
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