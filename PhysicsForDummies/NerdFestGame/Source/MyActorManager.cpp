#include "PhysGame\Source\ActorManager.h"
#include "MyActorManager.h"
#include "PhysicsEngine\Source\PhysEngine.h"
#include "PhysGame\Source\GameEngine.h"
#include "PhysGame\Source\GameWorld.h"
#include "MyEngine.h"
#include "GameObjects.h"
#include "Wall.h"
#include "Player.h"
body* am_createbody(PhysEngine* engine, float x, float y, float w, float h, float mass, float rot = 0){
	bodydef bdef;
	bdef.position.x = x;
	bdef.position.y = y;
	bdef.width = w;
	bdef.height = h;
	bdef.mass = mass;
	bdef.rotation = rot;
	return engine->buildBody(bdef);
}



Actor* MyActorManager::getActorByID(int id, float w, float h){
	PhysEngine* phys = m_game_engine->getGameWorld()->m_physEngine;
	if (id == 1){
		if (m_my_engine->m_player != NULL)
			return m_my_engine->m_player;
		body* mybody = am_createbody(phys, 0, 0, w, h, 0, 0);
		Player* p = new Player(1, mybody);
		m_game_engine->getGameWorld()->addActor(p);
		m_my_engine->m_player = p;
		return p;
	}
	return 0L;
}

Actor* MyActorManager::getActorByType(int type, float w, float h){
	return 0L;
}