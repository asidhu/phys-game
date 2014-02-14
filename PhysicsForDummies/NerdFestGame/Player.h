#pragma once

#include "Mob.h"
#define NULL 0
class Grapple;
class GameEngine;
class Player : public Mob{
public:
	int dmgfx;
	Grapple* grappleHook;
	GameEngine* engine;
	int onGround;
	Player(int id, body* b);
	void dmg(int d);
	void fireGrappleHook(float x, float y);
	void releaseGrappleHook();
	void fireSecondHook(float x, float y);
	void fireMissile(GameEngine*,float x, float y);
	virtual bool tick(GameEngine*);
	void render(RenderList*);
};