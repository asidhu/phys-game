#pragma once
#include "GameObjects.h"
#include "PhysicsEngine\Source\Body.h"
#define MOB_FLAG	0xf0000000
class body;
class RenderList;
class contactdetails;
class Mob :public GameObject{
public:
	float m_hp;
	int dmgfx;
	int m_deathframes;
	autoptr<GameObject>* target;
	static void checkExtremeCollision(Mob* m, contactdetails* cd);
	Mob(int id, body* b);
	virtual void dmg(float d){ dmgfx = 50; m_hp -= d; }
	virtual void render(RenderList*);
	virtual bool tick( float timestep, GameWorld*){
		if(dmgfx>0)dmgfx--;
		if (m_hp<=0 && m_deathframes++>300)
		{
		//	return true;
		}
		return false;
	}

	virtual void giveInfo(int infotype, int info){
		if (infotype == INFOTYPE_RESID)
			m_tex = info;
	}

	bool hasTarget(){
		if (target == NULL)
			return false;
		if (target->isAlive())
			return true;
		else{
			target->free();
			target = NULL;
			return false;
		}
	}
	void acquireTarget(GameObject* o){
		if (target == NULL){
			target = o->m_obj;
			target->get();
		}
	}
	void releaseTarget(){
		if (target != NULL){
			target->free();
			target = NULL;
		}
	}
};