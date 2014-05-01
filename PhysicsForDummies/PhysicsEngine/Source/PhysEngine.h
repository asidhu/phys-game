#pragma once
#include <vector>
#include "shape.h"
#include "collision.h"
#include "Body.h"
#include "BodyDef.h"
class body;
class solver;
struct bodydef;

typedef void (*bodysearchfunctor)(body* b, float r, vec2 diff);

class PhysEngine{
private:
	float gravity;
	float mX, mY, maxX, maxY;
	body root;
	solver* contacts;
	int m_numBodies;
	int velocityIterations, positionIterations;
	bool debug_enabled;
	bool debug_onCollisionStop;
	int debug_collidedState;

public:
	PhysEngine() :velocityIterations(25), positionIterations(4){ debug_enabled = false; }
	body* getBodyList(){ return root.nextX; }
	void setup(float g = -9.81f, float minX = 0.f, float minY = 0.f, float maxX = 1.f, float maxY = 1.f);
	body* buildBody(bodydef& def);
	int getNumBodies(){return m_numBodies;}
	void createBody(float x, float y, float r, float vx = 0.f, float vy = 0.f, float mass = 1.f, float restitution = 1.0f);
	void createBox(float x, float y, float hw, float hh, float vx = 0.f, float vy = 0.f, float mass = 1.f, float restitution = 1.0f, float rotate = 0.f);
	void step(float time);
	void removeBody(body*);
	void enableDebugger(bool stopOnCollision){ debug_enabled = true; debug_onCollisionStop = stopOnCollision; debug_collidedState = false; }
	void disableDebugger(){ debug_enabled = false; }
	void debug_collisionAdvance(){ debug_collidedState = 2; }

	void findAll(float x, float y,float radius, bodysearchfunctor fctn);

	contactdetails* debug_getCollisions();
	int	debug_getNumCollisions();
	float getGravity(){
		return gravity;
	}
	void setGravity(float f){
		gravity = f;
	}
};