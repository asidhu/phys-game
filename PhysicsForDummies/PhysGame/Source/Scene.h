#pragma once
#include <vector>
#include <map>
#include <cassert>
#include "Camera.h"
class RenderList;
class StaticFixture;
class PlatformGraphics;
class Actor;
class Layer;
class GameWorld;
class Scene{
private:
	RenderList* m_lists;
	Layer* m_layers;
	int m_numlayers;
	Camera m_Bounds;
	Camera m_mainCam;
	GameWorld* m_gameWorld;
	int m_sceneidx;
	char* m_lua_file;
	std::map<int, RenderList*> m_external_lists;
public:
	Scene(int IDX,int numLayers, char* file);
	~Scene();
	void addExternalList(int id, RenderList* l){
		assert(m_external_lists.find(id) == m_external_lists.end());
		m_external_lists[id] = l;
	}
	bool hasExternalList(int id){
		return m_external_lists.find(id) != m_external_lists.end();
	}
	RenderList* getExternalList(int id){
		assert(m_external_lists.find(id) != m_external_lists.end());
		return  m_external_lists[id];
	}
	void setGameWorld(GameWorld* w){ m_gameWorld = w; }
	GameWorld* getGameWorld(){ return m_gameWorld; }
	Camera* getCamera(){ return &m_mainCam; }
	Camera* getBounds(){ return &m_Bounds; }
	
	RenderList* render(int, Camera*);
	virtual void tick();
	int getNumLayers(){
		return m_numlayers;
	}
	char* getLuaFile(){ return m_lua_file; }
	Layer* getLayer(int layer);
};
