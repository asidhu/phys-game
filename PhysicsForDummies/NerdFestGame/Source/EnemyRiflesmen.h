#pragma once
#include "Mob.h"

class EnemyRiflesmen : public Mob{
	int fire;
	int jump;
public:
	EnemyRiflesmen(body*);
	void fireMissile(GameWorld* e, float x, float y);
	bool tick(GameWorld*);
};