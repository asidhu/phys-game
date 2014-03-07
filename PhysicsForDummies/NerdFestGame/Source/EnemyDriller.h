#pragma once
#include "Mob.h"

class EnemyDriller : public Mob{
	int fire;
	int jump;
public:
	EnemyDriller(body*);
	void render(RenderList*);
	bool tick(GameEngine*);
};