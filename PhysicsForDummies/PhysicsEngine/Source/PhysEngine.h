#pragma once
#include <vector>
#include "shape.h"
#include "collision.h"
#include "Body.h"
#include "BodyDef.h"
class body;
struct bodydef;
class PhysEngine{
private:
	float gravity;
	float mX, mY, maxX, maxY;
	body root;
	//std::vector<body*> bodies;
	int velocityIterations, positionIterations;
public:
	PhysEngine():velocityIterations(10), positionIterations(3){}
	//std::vector<body*>& getBodies(){ return bodies; }
	void setup(float g = -9.81f, float minX = 0.f, float minY = 0.f, float maxX = 1.f, float maxY = 1.f);
	body* buildBody(bodydef& def);
	void createBody(float x, float y, float r, float vx = 0.f, float vy = 0.f, float mass = 1.f, float restitution = 1.0f);
	void createBox(float x, float y, float hw, float hh, float vx = 0.f, float vy = 0.f, float mass = 1.f, float restitution = 1.0f, float rotate = 0.f);
	void step(float time);
	void removeBody(body*);
};