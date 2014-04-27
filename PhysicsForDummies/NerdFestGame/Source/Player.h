#pragma once

#include "Mob.h"
#define NULL 0
class Grapple;
class GameEngine;
class Player : public Mob{
public:
	Grapple* grappleHook;
	GameEngine* engine;
	int onGround;
	vec2 missileTo;
	vec2 grapplingHookTo;
	int createMissile, fireGrapplingHook, fire2ndGrapplingHook;

	Player(int id, body* b);
	void dmg(int d);
	void fireGrappleHook(float x, float y);
	void releaseGrappleHook();
	void fireSecondHook(float x, float y);
	void fireMissile(GameEngine*,float x, float y);
	virtual bool tick(GameWorld*);
	void render(RenderList*);
};