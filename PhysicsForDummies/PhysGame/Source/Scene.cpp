#include "Scene.h"
#include "PhysGame\Source\RenderList.h"
#include "PhysGame\Source\PlatformDependent\PlatformGraphics.h"
#include "PhysGame\Source\Actor.h"
#include "Layer.h"
#include <cassert>
Scene::Scene(int idx, int numLayers,char * file){
	m_numlayers = numLayers;
	m_lists = new RenderList[numLayers];
	m_layers = new Layer[numLayers];
	m_sceneidx = idx;
	m_lua_file = file;
}

Layer* Scene::getLayer(int layer){
	assert(layer >= 0 && layer < m_numlayers);
	return m_layers + layer;
}

RenderList* Scene::render(int layer, Camera* cam){
	assert(layer >= 0 && layer < m_numlayers);
	RenderList* list = m_lists + layer;
	list->clear();
	Layer* myLayer = m_layers + layer;
	myLayer->render(list, cam);
	return list;
}
void Scene::tick(){
	float offX = m_mainCam.l - m_Bounds.l,
		offY = m_mainCam.t - m_Bounds.t;
	//width of layer/width of scene
	//update layers scrolling.
	for (int i = 0; i < m_numlayers; i++){
		(m_layers + i)->updateScroll(offX,offY,&m_mainCam);
	}
}

Scene::~Scene(){
	delete m_lists;
	delete m_layers;
}