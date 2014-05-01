#pragma once

#include "Player.h"
#include "PhysGame\Source\GameEngine.h"
#include <vector>
class ProjectilePathRenderer;
class MyEngine{
	bool m_renderPaths;
public:
	Player* m_player;
	GameEngine* game_engine;
	PathRenderDetails path_rendering_specifications;
	bool slow_time;
	MyEngine();
	
	void slowTime(float newtimeStep);
	void slomo(bool);
	void setProjectilePathVisibility(bool on);
	void enableProjectilePath(GameObject* o);
};