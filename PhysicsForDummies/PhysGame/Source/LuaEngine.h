#pragma once

class Scene;
class GameEngine;
typedef struct lua_State lua_State;
class LuaEngine{
	GameEngine* engine;
	lua_State* m_state;
public:


	LuaEngine();
	void initiializeEngine(GameEngine* e){ engine = e; }
	void initializeLua(Scene*);
	void registerBindings(lua_State*);
	void registerGlobals(lua_State*);
	void tick(float timestep);
};