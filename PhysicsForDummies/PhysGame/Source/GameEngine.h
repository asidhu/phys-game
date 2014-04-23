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
class GameEngine{
private:
	PlatformGraphics* m_graphics;
	PlatformInput* m_input;
	GameWorld* m_world;
	SceneManager* m_scene_manager;
	GameInputHandler* m_inputHandler;
	RenderList* m_list, *m_debug_list;
	bool m_exit;
	float m_timestep;
public:
	SceneManager* getSceneManager(){	return m_scene_manager;			}
	GameWorld* getGameWorld(){			return m_world;					}

	GameEngine();
	void setup(PlatformGraphics* g, PlatformInput* i, GameInputHandler* handler){
		m_graphics = g;
		m_input = i;
		m_inputHandler = handler;
	}
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