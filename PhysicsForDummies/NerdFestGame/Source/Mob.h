#pragma once
#include "GameObjects.h"
#include "PhysicsEngine\Source\Body.h"
#define MOB_FLAG	0xf0000000
class body;
class RenderList;
class contactdetails;
class Mob :public GameObject{
public:
	unsigned int m_tex;
	int m_hp;
	int dmgfx;
	int m_deathframes;
	static void checkExtremeCollision(Mob* m, contactdetails* cd);
	Mob(int id, body* b);
	virtual void dmg(int d){ dmgfx = 50; m_hp -= d; }
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
};