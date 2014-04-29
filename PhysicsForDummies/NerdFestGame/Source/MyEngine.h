#pragma once

#include "Player.h"
#include "PhysGame\Source\GameEngine.h"
#include <vector>
class ProjectilePathRenderer;
class MyEngine{
public:
	Player* m_player;
	GameEngine* game_engine;
	bool projectilePathsVisible;
	//std::vector<ProjectilePathRenderer*> m_projectilepaths;
	MyEngine(){ m_player = 0L; projectilePathsVisible = false; }

	void slowTime(float newtimeStep);
	void setProjectilePathVisibility(bool on);
	void enableProjectilePath(GameObject* o);
};