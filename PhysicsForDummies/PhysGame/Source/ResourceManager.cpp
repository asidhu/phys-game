#include "ResourceManager.h"
#include "PlatformDependent\PlatformGraphics.h"
#include <cstring>
#include "Scene.h"

void ResourceManager::registerTexture(int id, const char* filename){
	assert(m_texture_resources.find(id) == m_texture_resources.end());
	m_graphics->loadImage(id, filename);
	TextureResource* tex = (TextureResource*)malloc(sizeof(TextureResource));
	strcpy(tex->filename, filename);
	m_texture_resources[id] = tex;
}

void ResourceManager::freeTextures(){
	std::map<int, TextureResource*>::iterator it = m_texture_resources.begin();
	for (; it != m_texture_resources.end(); it++){
		m_graphics->freeImage(it->first);
	}
	m_texture_resources.clear();
}

Scene* ResourceManager::getScene(int idx){
	assert(m_scene_resources.find(idx) != m_scene_resources.end());
	SceneResource* res = m_scene_resources.find(idx)->second;
	Scene* newScene = new Scene(idx,res->nLayers,res->filename);
	return newScene;
}


void ResourceManager::loadScenes(const char* filename){
	FILE* myFile = fopen(filename, "r");
	char fileData[8192];
	int size = fread(fileData, sizeof(char), 8192, myFile);
	fileData[size] = 0;
	char* line = strtok(fileData, "\n\r");
	while (line != NULL){
		int idx;
		int numLayers;
		char name[256];
		int sargs=sscanf(line, "%d,%d,%s",&idx,&numLayers,&name);
		assert(sargs == 3);
		assert(m_scene_resources.find(idx) == m_scene_resources.end());
		SceneResource *res = (SceneResource*)malloc(sizeof(SceneResource));
		res->idx = idx;
		res->nLayers = numLayers;
		strcpy(res->filename, name);
		m_scene_resources[idx] = res;
		line = strtok(NULL, "\n\r");
	}
	fclose(myFile);
}