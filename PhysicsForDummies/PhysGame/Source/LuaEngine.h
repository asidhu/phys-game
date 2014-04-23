#pragma once

typedef struct lua_State lua_State;
class LuaEngine{
	lua_State* m_state;
public:


	LuaEngine();

	void initializeLua();
	void registerBindings(lua_State*);
	
};