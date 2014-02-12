#include "GameEngine.h"
#include "Actor.h"
#include <PhysicsEngine\Source\PhysEngine.h>



//NOPERS SHOULDNT USE THIS :O
#include <GLFW\glfw3.h>


GameEngine::GameEngine(){

	//setup physics engine
	m_physEngine = new PhysEngine();
	m_physEngine->setup(-9.81);// no magic constants... oh well

	//setup renderer?
	//m_renderer.init();
}	

void GameEngine::tick(){
	m_physEngine->step(1.0f / 32.0f);//no magic constants!!! :(
	for (std::list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++){
		Actor* actor = *it;
		if (actor->tick())
			it = m_actors.erase(it);
	}
	inputTick();
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
		D_Down = (state != GLFW_RELEASE);
	}
	if (key == GLFW_KEY_A){
		A_Down = (state != GLFW_RELEASE);
	}
	if (key == GLFW_KEY_W){
		W_Down = (state != GLFW_RELEASE);
	}
	if (key == GLFW_KEY_S){
		S_Down = (state != GLFW_RELEASE);
	}
}

void GameEngine::handleMouse(int x, int y, int button, int state){

}

void GameEngine::handleMouseMove(int x, int y){

}
//void GameEngine::render()