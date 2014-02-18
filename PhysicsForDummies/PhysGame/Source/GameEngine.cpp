#include "GameEngine.h"
#include "Actor.h"
#include <PhysicsEngine\Source\PhysEngine.h>
#include "PlatformDependent\GLFWInput.h"
#include "PlatformDependent\GLFWGraphics.h"
#include "PhysGame\Source\RenderList.h"
#include "PhysGame\Source\Scene.h"
#include "Effects.h"

//this is a hack
#include "NerdFestGame\Player.h"
#include "NerdFestGame\Grapple.h"

#define MAXPLAYERSPEED 10
#define PROJECTILESPEED 30
GameEngine::GameEngine(){

	//setup physics engine
	m_physEngine = new PhysEngine();
	m_physEngine->setup(-9.81f);// no magic constants... oh well
	m_list = new RenderList();
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
			it++;
	}
	for (std::list<Effect*>::iterator it = m_effects.begin(); it != m_effects.end();){
		Effect* actor = *it;
		if (actor->tick()){
			it = m_effects.erase(it);
			delete actor;
		}
		else
			it++;
	}
	inputTick();
	m_input->pollInput();
}
vec2 move_speed(1.f, 0);
vec2 up_speed(0, 12);
vec2 fastfall_speed(0, -.8f);
void GameEngine::inputTick(){
	//this is hacked in
	if (D_Down &&  player->getBody()->velocity.x < MAXPLAYERSPEED){
		player->getBody()->velocity += (move_speed);
	}
	if (A_Down &&  player->getBody()->velocity.x > -MAXPLAYERSPEED){
		player->getBody()->velocity += (-1 * move_speed);
	}
	if (W_Down && ((Player*)player)->onGround>10){
		player->getBody()->velocity += (up_speed);
		((Player*)player)->onGround = 0;
	}
	if (S_Down && player->getBody()->velocity.y > -MAXPLAYERSPEED){
		player->getBody()->velocity+=(fastfall_speed);
	}

	if (RIGHT_Down);
		//((Player*)player)->fireGrappleHook(0, 0);
	else
		((Player*)player)->releaseGrappleHook();


}

void GameEngine::addActor(Actor* actor){
	m_actors.push_back(actor);
}



void GameEngine::handleKey(int key, int state){
	//hacked in
	if (key == 68){
		D_Down = (state != KEY_UP);
	}
	if (key == 65){
		A_Down = (state != KEY_UP);
	}
	if (key == 87){
		W_Down = (state != KEY_UP);
	}
	if (key == 83){
		S_Down = (state != KEY_UP);
	}
}



void GameEngine::handleMouse(float x, float y, int button, int state){
	//hacked in
	Player* p = (Player*)player;
	if (state == MOUSE_PRESS && button == MOUSE_LEFT){
		if (p->grappleHook == NULL || p->grappleHook->shouldreturn){
			p->fireMissile(this, x, y);
		}
		else{
			p->fireSecondHook(x, y);
		}
	}
	if (button == MOUSE_RIGHT){
		RIGHT_Down = state == MOUSE_PRESS;
		if (RIGHT_Down){
			p->fireGrappleHook(x, y);
		}
	}
}

void GameEngine::handleMouseMove(float x, float y){
	//hacked in

}
void GameEngine::render(){
	for (std::list<Effect*>::iterator it = m_effects.begin(); it != m_effects.end();){
		Effect* actor = *it;
		actor->render(m_list);
		it++;
	}
	for (std::list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++){
		Actor* actor = *it;
		actor->render(m_list);
	}

	m_graphics->start();
	m_graphics->drawList(m_scene->render(0));
	m_graphics->drawList(m_list);
	m_graphics->close();
	m_list->clear();
}