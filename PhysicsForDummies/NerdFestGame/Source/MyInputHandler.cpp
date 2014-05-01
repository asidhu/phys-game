#include "MyInputHandler.h"
#include "MyEngine.h"
#include "PhysGame\Source\PlatformDependent\PlatformInput.h"
#include "PhysGame\include\GLFW\glfw3.h"
#include "PhysGame\Source\GameEngine.h"
#include "PhysGame\Source\Scene.h"
#include "PhysGame\Source\SceneManager.h"
#include "PhysicsEngine\Source\Body.h"
#include "PhysGame\Source\GameWorld.h"
#include "Grapple.h"

void MyInputHandler::handleKey(int key, int state){
	if (m_keys.find(key) == m_keys.end()){
		m_keys[key] = 0;
	}
	if (state == KEY_DOWN)
		m_keys[key] = KEY_DOWN;
	if (state == KEY_UP)
		m_keys[key] = KEY_UP;
}

bool MyInputHandler::isDown(int key){
	return m_keys[key] == KEY_DOWN;
}
bool MyInputHandler::isUp(int key){
	return m_keys[key] == KEY_UP || m_keys[key]==0;
}
bool MyInputHandler::justReleased(int key){
	return m_keys[key] == KEY_UP;
}
void MyInputHandler::tick(float timestep){
	const vec2 move_speed = vec2(3, 0);
	const vec2 up_speed = vec2(0,30);
	const vec2 fastfall_speed = vec2(0, -1);
	Player* player = engine->m_player;
	if (isDown(GLFW_KEY_D) &&  player->getBody()->velocity.x < 30){
		player->getBody()->impulse += move_speed;
	}
	if (isDown(GLFW_KEY_A) && player->getBody()->velocity.x > -30){
		player->getBody()->impulse += move_speed* -1;
	}
	if (isDown(GLFW_KEY_W) && ((Player*)player)->onGround>30){
		player->getBody()->velocity += (up_speed);
		((Player*)player)->onGround = 0;
	}
	if (isDown(GLFW_KEY_S) && player->getBody()->velocity.y > -30){
		player->getBody()->velocity += (fastfall_speed);
	}

	if (isDown(GLFW_KEY_Q)){
		player->initiateGravityWell(player->mX, player->mY,20,true);
		
	}
	if (isDown(GLFW_KEY_E)){
		player->initiateGravityWell(player->mX, player->mY, 20, false);

	}

	if (isDown(GLFW_KEY_SPACE)){
		if (player->canSlowTime()){
			engine->slomo(true);
		}
	}
	else{
		engine->slomo(false);
	}

	if (RIGHT_DOWN==KEY_DOWN){
		//chargeTime += timestep;
		if (player ->grappleHook != NULL){
			player->releaseGrappleHook();
		}
	}
	else if(RIGHT_DOWN==KEY_UP){
		//if (chargeTime > .3f){
		(player)->fireGrappleHook(mX, mY);
			//chargeTime = 0;
		//}
	}
	if (LEFT_DOWN==KEY_UP){
		if (player->grappleHook == NULL || player->grappleHook->shouldreturn){
			player->fireMissile(engine->game_engine, mX, mY);
		}
		else{
			player->fireSecondHook(mX, mY);
		}
	}

	for (std::map<int, int>::iterator it = m_keys.begin(); it != m_keys.end(); it++)
		if(m_keys[it->first]==KEY_UP)m_keys[it->first] = 0;
	if (LEFT_DOWN == KEY_UP)
		LEFT_DOWN = 0;
	if (RIGHT_DOWN == KEY_UP)
		RIGHT_DOWN = 0;
}

void MyInputHandler::handleMouseMove(float x, float y){
	Player* p = (Player*)engine->m_player;
	Camera* cam = engine->game_engine->getSceneManager()->getCurrentScene()->getCamera();
	mX = x + cam->l;
	mY = y + cam->t;
	p->mX = mX;
	p->mY = mY;

}

void MyInputHandler::handleMouse(float x, float y, int button, int state){
	Player* p = (Player*)engine->m_player;
	Camera* cam = engine->game_engine->getSceneManager()->getCurrentScene()->getCamera();
	mX = x+cam->l;
	mY = y+cam->t;

	((button == MOUSE_LEFT) ? LEFT_DOWN : RIGHT_DOWN) = (state == MOUSE_PRESS) ? 1 : 2;

}