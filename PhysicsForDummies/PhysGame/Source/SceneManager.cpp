#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"
#include "LuaEngine.h"
#include "ResourceManager.h"


Scene* SceneManager::setupScene(int scene_id){
	if (currentScene != NULL)
		delete currentScene;
	currentScene = m_res_manager->getScene(scene_id);
	currentScene->setGameWorld(m_game_world);
	m_lua_engine->initializeLua(currentScene);
	return currentScene;
}

void SceneManager::tick(float timestep){
	m_lua_engine->tick(timestep);
	currentScene->tick();
}

void SceneManager::handleMouseUpdate(float x, float y){
	for (int i = 0; i < currentScene->getNumLayers(); i++){
		Layer* lyr = currentScene->getLayer(i);
		if(lyr->enabled)
			lyr->handleMouseUpdate(x, y);
	}
}

bool SceneManager::handleMouseClick(int type, float x, float y){
	for (int i = 0; i < currentScene->getNumLayers(); i++){
		Layer* lyr = currentScene->getLayer(i);
		if (lyr->enabled && lyr->handleMouseClick(type, x, y))
			return true;
	}
	return false;
}

SceneManager::SceneManager(){
	currentScene = NULL;
	m_lua_engine = new LuaEngine();
}