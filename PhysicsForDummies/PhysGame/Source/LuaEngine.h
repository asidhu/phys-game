#pragma once

class Scene;
class GameEngine;
typedef struct lua_State lua_State;

typedef void(*lua_binding_hook)(lua_State*);
class LuaEngine{
	GameEngine* engine;
	lua_State* m_state;
public:
	lua_binding_hook m_hook;

	LuaEngine();
	lua_State* getLuaState(){ return m_state; }
	void initiializeEngine(GameEngine* e){ engine = e; }
	void initializeLua(Scene*);
	void registerBindings(lua_State*);
	void registerGlobals(lua_State*);
	void tick(float timestep);
	void sendButtonEvent(int btnevt);
};