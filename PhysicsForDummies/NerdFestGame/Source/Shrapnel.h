#pragma once
#include "PhysGame\Source\Actor.h"
#include "GameObjects.h"
class Shrapnel :public GameObject{
public:
	float life;
	int dmg;
	float r, g, b;
	Shrapnel(int id, body* b);
	bool tick(float timestep, GameWorld*);
	void render(RenderList*);
};