#pragma once

#include "Mob.h"
#define NULL 0
class Grapple;
class GameEngine;
class MyEngine;
class Player : public Mob{
public:
	static Player* me;
	Grapple* grappleHook;
	MyEngine* engine;
	int onGround;
	vec2 missileTo;
	vec2 grapplingHookTo;
	int createMissile, fireGrapplingHook, fire2ndGrapplingHook;
	float mX, mY;	
	float psychicPowerMeter;
	bool mRenderProjectilePath;

	Player(int id, body* b);
	void dmg(int d);
	void fireGrappleHook(float x, float y);
	void releaseGrappleHook();
	void fireSecondHook(float x, float y);
	void fireMissile(GameEngine*,float x, float y);
	virtual bool tick(float timestep,GameWorld*);
	bool canSlowTime();
	bool canFireMissile();
	bool canFireSecondHook();
	bool canFireGrapplingHook();
	void initiateGravityWell(float wX, float wY,float radius, bool);
	static void explode(body*, float, vec2);
	static void implode(body*, float, vec2);
	static void targetme(body*, float, vec2);
	void render(RenderList*);
	void renderProjectilePath(RenderList* list);
	void renderPsychicBar(RenderList* list);
	void renderHealthBar(RenderList* list);
	bool cullActor(Camera* cam){
		return true;
	}
};