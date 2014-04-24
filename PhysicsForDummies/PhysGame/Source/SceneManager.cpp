#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"
void SceneManager::loadScenes(char* files){

}

Scene* SceneManager::setupScene(int scene_id){
	return NULL;
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