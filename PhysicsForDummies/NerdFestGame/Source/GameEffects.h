#pragma once
#include "PhysGame\Source\Effects.h"
#define SMOKELENGTH 5
class SmokeEffect :public Effect{
	float x, y;
	float dur;
public:
	SmokeEffect(float x, float y){ dur = 0; this->x = x; this->y = y; }
	bool tick(float timestep);
	void render(RenderList*);
};