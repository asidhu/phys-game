#pragma once
#include "autoptr.h"
#include "PhysGame\Source\Actor.h"
class Grapple;

#define OKMISSILE 0x0f000000
#define OKGRAPPLE 0x00F00000

struct PathRenderDetails{
	bool *shouldRender;
	float ti, tf, toff;
	float ai, af;
	int num;
	float half_gravity;
	bool *alive;
	union{
		struct{ float r, g, b; };
		float color[3];
	};
};

class GameObject :public Actor{
	public:
		PathRenderDetails* renderPathDetails;
		autoptr<GameObject> *m_obj;
		Grapple* m_grapple;
		GameObject(body* b, bool grapplable, bool missilable);
		void grapple(Grapple* m);
		virtual void release();
		void releaseHook();
		~GameObject(){}
		void renderPath(RenderList*,body* obj);
};