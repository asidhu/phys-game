#include "GameEngine.h"
#include "Actor.h"
#include "PlatformDependent\GLFWInput.h"
#include "PlatformDependent\GLFWGraphics.h"
#include "PhysGame\Source\RenderList.h"
#include "PhysGame\Source\Scene.h"
#include "Effects.h"
#include "GameWorld.h"
#include "SceneManager.h"

GameEngine::GameEngine(){
	m_list = new RenderList();
	m_debug_list = new RenderList();
	m_exit = false;
	m_scene_manager = new SceneManager();
	m_world = new GameWorld();
}	

void GameEngine::tick(){
	m_world->tick(m_timestep);
	m_scene_manager->currentScene->tick();
	/*

	*/
	m_input->pollInput();
}


void GameEngine::render(){
	Camera bounds;
	bounds.l = m_graphics->getLeft();
	bounds.r = m_graphics->getRight();
	bounds.t = m_graphics->m_top;
	bounds.b = m_graphics->m_bottom;

	m_graphics->start();
	Scene* scene = m_scene_manager->currentScene;
	for (int i = 0; i < scene->getNumLayers(); i++){
		RenderList* list = scene->render(i, &bounds);
		m_graphics->drawList(list);
	}
	//m_graphics->drawList(m_list);
	m_graphics->drawList(m_debug_list);
	m_graphics->close();
	m_list->clear();
	m_debug_list->clear();
}