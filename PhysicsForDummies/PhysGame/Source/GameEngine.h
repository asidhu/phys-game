#pragma once
#include <list>
class PhysEngine;
class PlatformGraphics;
class PlatformInput;
class Actor;
class RenderList;
class Scene;
class Effect;
class GameInputHandler;
class GameWorld;
class SceneManager;
class ActorManager;
class LuaEngine;
class ResourceManager;
class GameEngine{
private:
	PlatformGraphics* m_graphics;
	PlatformInput* m_input;
	GameWorld* m_world;
	ActorManager* m_actor_manager;
	SceneManager* m_scene_manager;
	ResourceManager* m_resource_manager;
	GameInputHandler* m_inputHandler;
	LuaEngine*	m_lua_engine;
	RenderList* m_list, *m_debug_list;
	bool m_exit;
	float m_timestep; //move into gameworld
public:
	SceneManager* getSceneManager(){	return m_scene_manager;			}
	GameWorld* getGameWorld(){			return m_world;					}
	ActorManager* getActorManager(){ return m_actor_manager; }
	LuaEngine*	getLuaEngine(){ return m_lua_engine; }
	ResourceManager* getResourceManager(){ return m_resource_manager; }

	GameEngine();
	void setup(PlatformGraphics* g, PlatformInput* i,
		GameInputHandler* handler,
		ActorManager* actor_manager);
	void close(){
		m_exit = true;
	}
	bool shouldClose(){
		return m_exit;
	}
	void tick();
	void render();
	RenderList* getDebugList(){
		return m_debug_list;
	}


	friend class PlatformInput;
	friend class PlatformGraphics;
};