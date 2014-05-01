#pragma once
#include "GameObjects.h"
class contactdetails;
class GameButton : public GameObject{
public:
	float setTime;
	float isHit;
	GameButton(body* b);
	static void toggle(body*, contactdetails*);
	bool tick(float timestep,GameWorld*);
	bool toggled();
	void render(RenderList*);
	void giveInfo(int infotype, int info){
		if (infotype == INFOTYPE_RESID)
			m_tex = info;
	}
};