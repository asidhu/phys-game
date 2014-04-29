#include "MyInputHandler.h"
#include "MyEngine.h"
#include "PhysGame\Source\PlatformDependent\PlatformInput.h"
#include "PhysGame\include\GLFW\glfw3.h"
#include "PhysGame\Source\GameEngine.h"
#include "PhysGame\Source\Scene.h"
#include "PhysGame\Source\SceneManager.h"
#include "PhysicsEngine\Source\Body.h"
#include "Grapple.h"

void MyInputHandler::handleKey(int key, int state){
	if (state == KEY_DOWN){

		GameEngine* e = engine->game_engine;
	}
	if (key == GLFW_KEY_D){
		D_DOWN = (state != KEY_UP);
	}
	if (key == GLFW_KEY_A){
		A_DOWN = (state != KEY_UP);
	}
	if (key == GLFW_KEY_W){
		W_DOWN = (state != KEY_UP);
	}
	if (key == GLFW_KEY_S){
		S_DOWN = (state != KEY_UP);
	}
	if (key == GLFW_KEY_Z){
		Z_DOWN = (state != KEY_UP);
	}
}

void MyInputHandler::tick(float timestep){
	const vec2 move_speed = vec2(10, 0);
	const vec2 up_speed = vec2(0,20);
	const vec2 fastfall_speed = vec2(0, -1);
	Player* player = engine->m_player;
	if (D_DOWN &&  player->getBody()->velocity.x < 20){
		player->getBody()->impulse += move_speed;
	}
	if (A_DOWN &&  player->getBody()->velocity.x > -20){
		player->getBody()->impulse += move_speed* -1;
	}
	if (W_DOWN && ((Player*)player)->onGround>10){
		player->getBody()->velocity += (up_speed);
		((Player*)player)->onGround = 0;
	}
	if (S_DOWN && player->getBody()->velocity.y > -5){
		player->getBody()->velocity += (fastfall_speed);
	}
	if (Z_DOWN){
		if (player->canSlowTime()){
			engine->slowTime(1.f / 240.f);
			engine->setProjectilePathVisibility(true);
		}
		else{
			engine->slowTime(1.f / 60.f);
			engine->setProjectilePathVisibility(false);
		}
	}
	else{
		engine->slowTime(1.f / 60.f);
		engine->setProjectilePathVisibility(false);
	}

	if (RIGHT_DOWN){
		chargeTime += timestep;
		if (player ->grappleHook != NULL){
			player->releaseGrappleHook();
		}
	}
	else{
		if (chargeTime > .3f){
			(player)->fireGrappleHook(mX, mY);
			chargeTime = 0;
		}
	}
}


void MyInputHandler::handleMouse(float x, float y, int button, int state){
	Player* p = (Player*)engine->m_player;
	mX = x;
	mY = y;
	if (state == MOUSE_PRESS && button == MOUSE_LEFT){
		if (p->grappleHook == NULL || p->grappleHook->shouldreturn){
			p->fireMissile(engine->game_engine, x, y);
		}
		else{
			p->fireSecondHook(x, y);
		}
	}
	if (button == MOUSE_RIGHT){
		RIGHT_DOWN = state == MOUSE_PRESS;
		if (RIGHT_DOWN){
			chargeTime = 0;
		}
	}
}