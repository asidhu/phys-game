#pragma once
#ifndef NULL
	#define NULL 0
#endif 
#include "math.h"
#include "rect.h"
class contactdetails;
class shape;
class body{
	
public:
	body(){
		linearDamping = .9995f;
		angularDamping = .9995f;
		nextX = prevX = nextY = prevY = NULL;
		pre_collide =  NULL;
		post_collide = NULL;
		dataFlag = 0;
	}
	vec2 position, velocity;
	vec2 impulse, accumulatedImpulse;
	shape* form;
	float restitution;
	float mass;
	float invMass;
	float rotation;
	float angularVelocity;
	float coeffFriction;
	float instantTorque, accumTorque;
	float invMomentInertia;
	float linearDamping, angularDamping;
	rect AABB, boundingArea;
	body* nextX, *prevX;
	body* nextY, *prevY;
	int dataFlag;
	void* data;


	//might want to rethink this?
	bool(*pre_collide)(body*,contactdetails*);
	void(*post_collide)(body*, contactdetails*);

	void applyImpulse(vec2 impulse){
		this->impulse += impulse;
	}
	void applyImpulse(){
		velocity += impulse*invMass;
		angularVelocity += instantTorque *invMomentInertia;
		instantTorque = 0;
		impulse.x = impulse.y = 0;
	}
	void dampen(){
		velocity = velocity*linearDamping;
		angularVelocity *= angularDamping;
	}

	void lockRotation(){
		invMomentInertia = 0;
	}
	vec2 velocityRelative2Point(vec2& point){
		vec2 out;
		out.x = -angularVelocity*point.y;
		out.y = angularVelocity*point.x;
		return out + velocity;
	}
	vec2 velocityRelative2PointAccum(vec2& point){
		vec2 out;
		float tmp = angularVelocity + accumTorque*invMomentInertia;
		out.x = -tmp*point.y;
		out.y = tmp*point.x;
		return out + velocity+(accumulatedImpulse*invMass);
	}
	void insertX(body* b){
		if (b->nextX != NULL)
			b->nextX->prevX = this;
		this->nextX = b->nextX;
		this->prevX = b;
		b->nextX = this;
	}

	void deleteX(){
		if (nextX != NULL)
			nextX->prevX = prevX;
		if (prevX != NULL)
			prevX->nextX = nextX;
	}
};