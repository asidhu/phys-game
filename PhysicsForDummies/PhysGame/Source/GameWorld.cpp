#include "GameWorld.h"
#include "Actor.h"
#include <PhysicsEngine\Source\PhysEngine.h>
#include "Effects.h"
#include "Camera.h"

GameWorld::GameWorld(){
	//setup physics engine
	m_physEngine = new PhysEngine();
	m_physEngine->setup(-9.81f);// no magic constants... oh well
}


void GameWorld::tick(float timestep){
	m_physEngine->step(timestep);
	for (std::list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end();){
		Actor* actor = *it;
		if (actor->tick(this)){
			m_physEngine->removeBody(actor->getBody());
			it = m_actors.erase(it);
			delete actor;
		}
		else
			++it;
	}
	for (std::list<Effect*>::iterator it = m_effects.begin(); it != m_effects.end();){
		Effect* actor = *it;
		if (actor->tick()){
			it = m_effects.erase(it);
			delete actor;
		}
		else
			++it;
	}
}


void GameWorld::render(RenderList* list,Camera* viewport, Camera* window){
	for (std::list<Effect*>::iterator it = m_effects.begin(); it != m_effects.end(); it++){
		Effect* actor = *it;
		actor->render(list);
	}
	for (std::list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++){
		Actor* actor = *it;
		if (actor->cullActor(viewport)){
			actor->render(list);
		}
	}
}