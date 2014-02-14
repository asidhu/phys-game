#pragma once
#include <list>
class PhysEngine;
class PlatformGraphics;
class PlatformInput;
class Actor;
class RenderList;
class Scene;
class Effect;
class GameEngine{
private:
	PhysEngine* m_physEngine;
	PlatformGraphics* m_graphics;
	PlatformInput* m_input;
	std::list<Effect*> m_effects;
	std::list<Actor*> m_actors;
	RenderList* m_list;
	Scene* m_scene;
	bool m_exit;
public:
	GameEngine();
	void setup(PlatformGraphics* g, PlatformInput* i){
		m_graphics = g;
		m_input = i;
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

	//these are hacks - make input manager!
	void handleKey(int key, int state);
	void handleMouse(float x, float y,int button, int state);
	void handleMouseMove(float x, float y);
	void inputTick();

	
	bool W_Down;
	bool A_Down;
	bool S_Down;
	bool D_Down;
	bool RIGHT_Down;


	//hacksss
	Actor* player;
	//void render();
	friend class PlatformInput;
	friend class PlatformGraphics;
};