#pragma once

class Scene;
typedef struct lua_State lua_State;
class LuaEngine{
	Scene* m_scene;
	lua_State* m_state;
public:


	LuaEngine();

	void initializeLua(Scene*);
	void registerBindings(lua_State*);
	
};