#include "GameEngine.h"
#include "Actor.h"
#include "PlatformDependent\GLFWInput.h"
#include "PlatformDependent\GLFWGraphics.h"
#include "PhysGame\Source\RenderList.h"
#include "PhysGame\Source\Scene.h"
#include "Effects.h"
#include "GameWorld.h"
#include "SceneManager.h"
#include "LuaEngine.h"
#include "ResourceManager.h"
#include "Layer.h"
#include "GameInputHandler.h"

GameEngine::GameEngine(){
	m_list = new RenderList();
	m_debug_list = new RenderList();
	m_exit = false;
	m_scene_manager = new SceneManager();
	m_world = new GameWorld();
	m_resource_manager = new ResourceManager();
	m_world->m_engine = this;
	m_scene_manager->setupResources(m_resource_manager);
	m_scene_manager->setGameWorld(m_world);
	m_scene_manager->getLuaEngine()->initiializeEngine(this);
	m_timestep = 1.0f / 60;
}	

void GameEngine::tick(){
	m_world->tick(m_timestep);
	m_inputHandler->tick(m_timestep);
	m_scene_manager->tick(m_timestep);
	m_input->pollInput();
}

void GameEngine::setup(PlatformGraphics* g, PlatformInput* i,
	GameInputHandler* handler, ActorManager* actor_manager){
	m_graphics = g;
	m_input = i;
	m_inputHandler = handler;
	m_actor_manager = actor_manager;
	m_resource_manager->setupManager(g);
}


void GameEngine::start(char* sceneFile){
	m_resource_manager->loadScenes(sceneFile);
	m_scene_manager->setupScene(0);
}
void GameEngine::render(){
	Camera bounds;
	bounds.l = m_graphics->getLeft();
	bounds.r = m_graphics->getRight();
	bounds.t = m_graphics->m_top;
	bounds.b = m_graphics->m_bottom;

	m_graphics->start();
	Scene* scene = m_scene_manager->getCurrentScene();
	for (int i = 0; i < scene->getNumLayers(); i++){
		if (scene->getLayer(i)->enabled){
			RenderList* list = scene->render(i, &bounds);
			m_graphics->drawList(list);
		}
	}
	//m_graphics->drawList(m_list);
	m_graphics->drawList(m_debug_list);
	m_graphics->close();
	m_list->clear();
	m_debug_list->clear();
}