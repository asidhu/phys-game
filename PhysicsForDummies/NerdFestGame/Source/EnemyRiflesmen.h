#pragma once
#include "Mob.h"

class EnemyRiflesmen : public Mob{
	float fire;
public:
	EnemyRiflesmen(body*);
	void fireMissile(GameWorld* e, float x, float y);
	bool tick(float timestep, GameWorld*);
};