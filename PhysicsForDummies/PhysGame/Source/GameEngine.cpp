#include "GameEngine.h"
#include "Actor.h"
#include <PhysicsEngine\Source\PhysEngine.h>
#include "PlatformDependent\GLFWInput.h"
#include "PlatformDependent\GLFWGraphics.h"
#include <GLFW\glfw3.h>
#include "PhysGame\Source\RenderList.h"
#include "PhysGame\Source\Scene.h"
#define MAXPLAYERSPEED 4
GameEngine::GameEngine(){

	//setup physics engine
	m_physEngine = new PhysEngine();
	m_physEngine->setup(-9.81f);// no magic constants... oh well
	m_list = new RenderList();
	m_exit = false;
}	

void GameEngine::tick(){
	m_physEngine->step(1.0f / 32.0f);//no magic constants!!! :(
	for (std::list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++){
		Actor* actor = *it;
		if (actor->tick())
			it = m_actors.erase(it);
	}
	inputTick();
	m_input->pollInput();
}
vec2 move_speed(.2f, 0);
vec2 up_speed(0, 8);
vec2 fastfall_speed(0, -.2f);
void GameEngine::inputTick(){
	if (D_Down &&  player->getBody()->velocity.x < MAXPLAYERSPEED){
		player->getBody()->applyImpulse(move_speed);
	}
	if (A_Down &&  player->getBody()->velocity.x > -MAXPLAYERSPEED){
		player->getBody()->applyImpulse(-1 * move_speed);
	}
	if (W_Down && player->onGround){
		player->getBody()->applyImpulse(up_speed);
		player->onGround = false;
	}
	if (S_Down && player->getBody()->velocity.y > -MAXPLAYERSPEED){
		player->getBody()->applyImpulse(fastfall_speed);
	}
}

void GameEngine::addActor(Actor* actor){
	m_actors.push_back(actor);
}



void GameEngine::handleKey(int key, int state){
	if (key == GLFW_KEY_D){
		D_Down = (state != KEY_UP);
	}
	if (key == GLFW_KEY_A){
		A_Down = (state != KEY_UP);
	}
	if (key == GLFW_KEY_W){
		W_Down = (state != KEY_UP);
	}
	if (key == GLFW_KEY_S){
		S_Down = (state != KEY_UP);
	}
}

void GameEngine::handleMouse(float x, float y, int button, int state){

}

void GameEngine::handleMouseMove(float x, float y){

}
void GameEngine::render(){
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