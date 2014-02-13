#include "Scene.h"
#include "PhysGame\Source\RenderList.h"
#include "PhysGame\Source\PlatformDependent\PlatformGraphics.h"
#include "PhysGame\Source\StaticFixture.h"
Scene::Scene(int numLayers){
	m_numlayers = numLayers;
	m_lists = new RenderList[numLayers];
}


RenderList* Scene::render(int layer){
	m_lists[layer].clear();
	for (std::list<StaticFixture*>::iterator it = m_fixtures.begin(); it != m_fixtures.end(); it++){
		StaticFixture* fix = *it;
		//TODO: fix the renderer based on layer.
		fix->render(m_lists + layer);
	}
	return m_lists + layer;
}
void Scene::addFixture(StaticFixture* fix, int layer){
	fix->m_zdist = layer;
	m_fixtures.push_back(fix);
}