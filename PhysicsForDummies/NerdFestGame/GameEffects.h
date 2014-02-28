#pragma once
#include "PhysGame\Source\Effects.h"
#define SMOKELENGTH 20
class SmokeEffect :public Effect{
	float x, y;
	int dur;
public:
	SmokeEffect(float x, float y){ dur = 0; this->x = x; this->y = y; }
	bool tick();
	void render(RenderList*);
};