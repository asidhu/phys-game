#include "MyInputHandler.h"
#include "MyEngine.h"
#include "PhysGame\Source\PlatformDependent\PlatformInput.h"
#include "PhysGame\include\GLFW\glfw3.h"
#include "PhysGame\Source\GameEngine.h"
#include "PhysGame\Source\Scene.h"
#include "PhysGame\Source\SceneManager.h"

void MyInputHandler::handleKey(int key, int state){
	if (state == KEY_DOWN){
		float scroll = 0.f;
		if (key == GLFW_KEY_LEFT){
			scroll = 1.f;
		}
		if (key == GLFW_KEY_RIGHT){
			scroll = -1.f;
		}
		GameEngine* e = engine->game_engine;
		Scene *s = e->getSceneManager()->currentScene;
		Camera* cam = s->getCamera();
 		cam->l += scroll;
		cam->r += scroll;
	}
}