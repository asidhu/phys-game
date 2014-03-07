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
class GameEngine{
private:
	PhysEngine* m_physEngine;
	PlatformGraphics* m_graphics;
	PlatformInput* m_input;
	GameInputHandler* m_inputHandler;
	std::list<Effect*> m_effects;
	std::list<Actor*> m_actors;
	RenderList* m_list, *m_debug_list;
	Scene* m_scene;
	bool m_exit;
public:
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
	void setupScene(Scene* s){
		m_scene = s;
	}
	void addEffect(Effect* e){
		m_effects.push_back(e);
	}
	void tick();
	void render();
	void addActor(Actor*);
	PhysEngine* getPhysEngine(){ return m_physEngine; } //THIS IS A HACK. GET RID OF LATER.
	int getNumActors(){ return m_actors.size(); }
	int getNumEffects(){ return m_effects.size(); }
	RenderList* getDebugList(){
		return m_debug_list;
	}


	friend class PlatformInput;
	friend class PlatformGraphics;
};