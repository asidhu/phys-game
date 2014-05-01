#pragma once
#include "autoptr.h"
#include "PhysGame\Source\Actor.h"
class Grapple;

#define OKMISSILE		0x0f000000
#define OKGRAPPLE		0x00F00000
#define OKGRAVITYWELL	0x000F0000
#define ISENEMY			0X0000F000
#define TRIGGERBUTTON	0x00000F00
#define INFOTYPE_RESID 10
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
		unsigned int m_tex;
		PathRenderDetails* renderPathDetails;
		autoptr<GameObject> *m_obj;
		Grapple* m_grapple;
		GameObject(body* b, bool grapplable, bool missilable);
		void setAffectedByGravityWell(bool g);
		void grapple(Grapple* m);
		virtual void release();
		void releaseHook();
		~GameObject(){}
		void renderPath(RenderList*,body* obj);
		virtual void giveInfo(int infotype, int info){}
};