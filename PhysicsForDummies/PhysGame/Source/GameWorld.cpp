#include "GameWorld.h"
#include "Actor.h"
#include <PhysicsEngine\Source\PhysEngine.h>
#include "Effects.h"
#include "Camera.h"
#include "GameEngine.h"
#include "mem\MemAccessPool.h"
#include "ActorManager.h"
GameWorld::GameWorld(){
	//setup physics engine
	m_physEngine = new PhysEngine();
	m_physEngine->setup(-9.81f);// no magic constants... oh well
	m_effects_mempool = new MemAccessPool(128*1024);//128k? why not
	m_actors_mempool = new MemAccessPool(128 * 1024);//128k? WHY NOT. 
}
void GameWorld::addActor(Actor* a){
	m_engine->getActorManager()->handleNewActor(a);
	assert(m_actors_mempool->own(a));
}

void GameWorld::addEffect(Effect* e){//this is for alerting actor manager of new data.
	assert(m_effects_mempool->own(e));
}
void* GameWorld::allocateEffect(int sz){
	return m_effects_mempool->allocate(sz);
}
void* GameWorld::allocateActor(int sz){
	return m_actors_mempool->allocate(sz);
}




void GameWorld::tick(float timestep){
	m_physEngine->step(timestep);
	MemAccessPool::iterator end = m_actors_mempool->end();
	for (MemAccessPool::iterator it = m_actors_mempool->begin(); it != end;){
		Actor* actor = (Actor*)*it;
		if (actor->tick(timestep, this)){
			actor->release();
			m_physEngine->removeBody(actor->getBody());
			it = m_actors_mempool->erase(it);
		}
		else
			++it;
	}
	for (MemAccessPool::iterator it = m_effects_mempool->begin(); it != end;){
		Effect* actor = (Effect*)*it;
		
		if (actor->tick(timestep)){
			actor->release();
			it = m_effects_mempool->erase(it);
		}
		else
			++it;
	}
}


void GameWorld::render(RenderList* list,Camera* viewport, Camera* window){
	for (MemAccessPool::iterator it = m_effects_mempool->begin(); it != m_effects_mempool->end(); it++){
		Effect* actor =(Effect*) *it;
		actor->render(list);
	}
	for (MemAccessPool::iterator it = m_actors_mempool->begin(); it != m_actors_mempool->end(); it++){
		Actor* actor = (Actor*)*it;
		if (actor->cullActor(viewport)){
			actor->render(list);
		}
	}
}