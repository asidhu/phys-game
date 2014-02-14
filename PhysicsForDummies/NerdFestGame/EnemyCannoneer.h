#pragma once
#include "Mob.h"

class EnemyCannoneer : public Mob{
	int fire;
public:
	EnemyCannoneer(body*);
	void fireMissile(GameEngine* e,float x, float y);
	bool tick(GameEngine*);
};