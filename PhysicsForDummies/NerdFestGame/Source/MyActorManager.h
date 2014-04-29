#pragma once

#include "PhysGame\Source\ActorManager.h"
class GameEngine;
class MyEngine;
class PhysEngine;
class Player;
class body;
class MyActorManager : public ActorManager{

public:
	Player* m_player; //Actor ID 1 
	GameEngine* m_game_engine;
	MyEngine * m_my_engine;
	MyActorManager(){
		m_player = 0L;
	}
	//Gets or creates actor of this actorType? probably creates.
	Actor* getActorByType(int actorType,float w, float h);
	//Gets or creates actor of this unique ID
	Actor* getActorByID(int actorID, float w, float h);
	void handleNewActor(Actor*);
	void tick(float timestep);
};

body* am_createbody(PhysEngine* engine, float x, float y, float w, float h, float mass, float rot);