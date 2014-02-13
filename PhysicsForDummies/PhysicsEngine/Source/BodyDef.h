#pragma once
#include "math.h"
struct bodydef{
	void* data;
	vec2 position;
	vec2 velocity;
	float rotation;
	float mass;
	float width, height;
	float restitution;
	float linearDamping, angularDamping;
	float coeffFriction;
	bodydef(){
		position.x = position.y = 0;
		velocity.x = velocity.y = 0;
		rotation = 0;
		mass = 1;
		width = height = 1;
		restitution = 0;
		linearDamping = angularDamping = .999998f;
		coeffFriction = .1f;
		data = NULL;
	}
};