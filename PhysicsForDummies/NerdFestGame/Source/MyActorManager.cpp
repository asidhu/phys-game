#include "PhysGame\Source\ActorManager.h"
#include "MyActorManager.h"
#include "PhysicsEngine\Source\PhysEngine.h"
#include "PhysGame\Source\GameEngine.h"
#include "PhysGame\Source\GameWorld.h"
#include "MyEngine.h"
#include "GameObjects.h"
#include "Wall.h"
#include "Player.h"
#include "EnemyRiflesmen.h"
#include "GameButton.h"
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
		Player* p = m_game_engine->getGameWorld()->allocateActor<Player>(1, mybody);
		m_game_engine->getGameWorld()->addActor(p);
		m_my_engine->m_player = p;
		p->engine = m_my_engine;
		return p;
	}
	return 0L;
}

Actor* MyActorManager::getActorByType(int type, float w, float h){
	PhysEngine* phys = m_game_engine->getGameWorld()->m_physEngine;
	if (type == 1){
		body* mybody = am_createbody(phys, 0, 0, w, h, 0, 0);
		EnemyRiflesmen* p = m_game_engine->getGameWorld()->allocateActor<EnemyRiflesmen>( mybody);
		m_game_engine->getGameWorld()->addActor(p);
		return p;
	}
	if (type == 2){
		body* mybody = am_createbody(phys, 0, 0, w, h, 0, 0);
		GameButton* p = m_game_engine->getGameWorld()->allocateActor<GameButton>(mybody);
		m_game_engine->getGameWorld()->addActor(p);
		return p;
	}
	return 0L;
}

void MyActorManager::handleNewActor(Actor* a){
	GameObject* o = (GameObject*)a;
	o->renderPathDetails = &(m_my_engine->path_rendering_specifications);

}

void MyActorManager::tick(float t){
	m_my_engine->path_rendering_specifications.toff += t;
	if (m_my_engine->path_rendering_specifications.toff > .2f)
		m_my_engine->path_rendering_specifications.toff -= .2f;
}