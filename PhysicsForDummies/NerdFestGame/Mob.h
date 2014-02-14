#pragma once
#include "Grappleable.h"
#include "GameObjects.h"
#define MOB_FLAG	0xf0000000
class body;
class RenderList;
class Mob :public GameObject{
public:
	unsigned int m_tex;
	int m_hp;
	int m_deathframes;
	Mob(int id, body* b);
	virtual void dmg(int d){ m_hp -= d; }
	virtual void render(RenderList*);
	virtual bool tick(GameEngine*){
		if (m_hp<=0 && m_deathframes++>300)
		{
			return true;
		}
		return false;
	}
};