#pragma once

#include "PhysGame\Source\Actor.h"
#include "GameObjects.h"
#define LASTPOS 8
class Missile :public GameObject{
public:
	float life;
	float dmg;
	GameObject* launcher;
	Missile(int id, body* b);
	bool tick(float timestep, GameWorld*);
	void render(RenderList*);
};