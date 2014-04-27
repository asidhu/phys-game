#include "MyInputHandler.h"
#include "MyEngine.h"
#include "PhysGame\Source\PlatformDependent\PlatformInput.h"
#include "PhysGame\include\GLFW\glfw3.h"
#include "PhysGame\Source\GameEngine.h"
#include "PhysGame\Source\Scene.h"
#include "PhysGame\Source\SceneManager.h"

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
}

void MyInputHandler::tick(float timestep){
	const vec2 move_speed = vec2(1,0 );
	Player* player = engine->m_player;
	if (D_DOWN &&  player->getBody()->velocity.x < 30){
		player->getBody()->impulse += move_speed;
		//player->getBody()->velocity += (move_speed);
	}
	if (A_DOWN &&  player->getBody()->velocity.x > -30){
		player->getBody()->impulse += move_speed* -1;
		//player->getBody()->velocity += (-1 * move_speed);
	}
	if (W_DOWN && ((Player*)player)->onGround>10){
		//player->getBody()->velocity += (up_speed);
		((Player*)player)->onGround = 0;
	}
	if (S_DOWN && player->getBody()->velocity.y > -5){
		//player->getBody()->velocity += (fastfall_speed);
	}
	if (RIGHT_DOWN);
	//((Player*)player)->fireGrappleHook(0, 0);
	else
		((Player*)player)->releaseGrappleHook();
}


void MyInputHandler::handleMouse(float x, float y, int button, int state){
	Player* p = (Player*)engine->m_player;
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
			p->fireGrappleHook(x, y);
		}
	}
}