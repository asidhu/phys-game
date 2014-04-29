#pragma once
#include "PhysGame\Source\Effects.h"
#include "Missile.h"
class RenderList;
class ProjectilePathRenderer :public Effect{
public:
	float ti, tf, toff;
	float ai, af;
	int num;
	float half_gravity;
	bool *alive;
	union{
		struct{ float r, g, b; };
		float color[3];
	};
	ProjectilePathRenderer(GameObject*);
	autoptr<GameObject> *target;
	bool tick(float timestep);
	void render(RenderList*);
};