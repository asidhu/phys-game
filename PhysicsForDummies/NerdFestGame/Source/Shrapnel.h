#pragma once
#include "PhysGame\Source\Actor.h"

class Shrapnel :public Actor{
public:
	int life;
	int dmg;
	float r, g, b;
	Shrapnel(int id, body* b);
	bool tick(GameEngine*);
	void render(RenderList*);
};