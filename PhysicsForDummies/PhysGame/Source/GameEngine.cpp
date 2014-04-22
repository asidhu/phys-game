#include "GameEngine.h"
#include "Actor.h"
#include <PhysicsEngine\Source\PhysEngine.h>
#include "PlatformDependent\GLFWInput.h"
#include "PlatformDependent\GLFWGraphics.h"
#include "PhysGame\Source\RenderList.h"
#include "PhysGame\Source\Scene.h"
#include "Effects.h"


GameEngine::GameEngine(){
	//setup physics engine
	m_physEngine = new PhysEngine();
	m_physEngine->setup(-9.81f);// no magic constants... oh well
	m_list = new RenderList();
	m_debug_list = new RenderList();
	m_exit = false;
}	

void GameEngine::tick(){
	m_physEngine->step(1.0f / 60.0f);//no magic constants!!! :(
	m_scene->tick();
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
	m_input->pollInput();
}

void GameEngine::addActor(Actor* actor){
	m_actors.push_back(actor);
}

void GameEngine::render(){
	for (std::list<Effect*>::iterator it = m_effects.begin(); it != m_effects.end();it++){
		Effect* actor = *it;
		actor->render(m_list);
	}
	for (std::list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++){
		Actor* actor = *it;
		actor->render(m_list);
	}

	m_graphics->start();
	//m_graphics->drawList(m_scene->render(0));
	m_graphics->drawList(m_list);
	m_graphics->drawList(m_debug_list);
	m_graphics->close();
	m_list->clear();
	m_debug_list->clear();
}