#pragma once

#include "Player.h"
#include "PhysGame\Source\GameEngine.h"

class MyEngine{
public:
	Player* m_player;
	GameEngine* game_engine;
	MyEngine(){ m_player = 0L; }
};