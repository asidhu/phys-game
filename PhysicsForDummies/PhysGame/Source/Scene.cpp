#include "Scene.h"
#include "PhysGame\Source\RenderList.h"
#include "PhysGame\Source\PlatformDependent\PlatformGraphics.h"
#include "PhysGame\Source\Actor.h"
#include "Layer.h"
#include <cassert>
Scene::Scene(int numLayers){
	m_numlayers = numLayers;
	m_lists = new RenderList[numLayers];
	m_layers = new Layer[numLayers];
}


RenderList* Scene::render(int layer, Camera* cam){
	assert(layer >= 0 && layer < m_numlayers);
	Layer* myLayer = m_layers + layer;
	myLayer->render(m_lists + layer, cam);
	return m_lists + layer;
}
void Scene::tick(){

}