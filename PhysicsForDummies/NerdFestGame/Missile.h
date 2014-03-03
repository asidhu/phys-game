#pragma once

#include "PhysGame\Source\Actor.h"
#include "GameObjects.h"
#define LASTPOS 8
class Missile :public GameObject{
public:
	int life;
	int dmg;
	GameObject* launcher;
	Missile(int id, body* b);
	bool tick(GameEngine*);
	void render(RenderList*);
};