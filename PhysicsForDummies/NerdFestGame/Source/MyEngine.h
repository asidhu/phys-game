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
	MyEngine();

	void slowTime(float newtimeStep);
	void setProjectilePathVisibility(bool on);
	void enableProjectilePath(GameObject* o);
};