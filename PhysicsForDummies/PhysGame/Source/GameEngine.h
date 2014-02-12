#pragma once
#include "opengl\primitives.h"
#include <list>
class PhysEngine;
class Actor;

#define MAXPLAYERSPEED 4 

class GameEngine{
private:
	PhysEngine* m_physEngine;
	primitives m_renderer; //replace with a graphics engine ricky :O
	std::list<Actor*> m_actors;

public:
	GameEngine();
	void tick();
	void addActor(Actor*);

	PhysEngine* getPhysEngine(){ return m_physEngine; } //THIS IS A HACK. GET RID OF LATER.
	//these are hacks - make input manager!
	void handleKey(int key, int state);
	void handleMouse(int x, int y,int button, int state);
	void handleMouseMove(int x, int y);
	void inputTick();
	
	bool W_Down;
	bool A_Down;
	bool S_Down;
	bool D_Down;

	//hacksss
	Actor* player;
	//void render();
};