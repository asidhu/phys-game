#pragma once
#include "autoptr.h"
#include "PhysGame\Source\Actor.h"
class Grapple;

#define OKMISSILE 0x0f000000
#define OKGRAPPLE 0x00F00000
class GameObject :public Actor{
	public:
		autoptr<GameObject> *m_obj;
		Grapple* m_grapple;
		GameObject(body* b, bool grapplable, bool missilable);
		void grapple(Grapple* m);
		void release();
		virtual ~GameObject();
};