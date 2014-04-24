#include "LuaEngine.h"
#include <lua.hpp>
#include "Layer.h"
#include "Scene.h"
LuaEngine::LuaEngine(){
	m_state = 0L;
}

void LuaEngine::initializeLua(Scene* s){
	if (m_state != 0L){
		lua_close(m_state);
	}
	m_state = luaL_newstate();
	registerBindings(m_state);
}
/*
What does LUA need to do?
Scene Management
	Construct Scene
		Load resources
			-reqs: resource manager
		Setup Game World
			-reqs: GameWorld, Actor, ActorManager
		Setup Layers
			-reqs: Scene, Layer
	Scene Update
		perform updates during game
			-reqs: GameWorld, Actor, ActorManager
		perform updates during cutscenes
			-reqs: Gameworld, actor, InputHandler, Scene
		enable cutscene/disable cutscene aka setup gameworld state
			-reqs: scene, gameworld
	Button Handler
		handles button actions
			-reqs: scenemanager, lots of stuff.
*/

int l_Scene_getLayer(lua_State* L){
	/*
		stack should be a Scene*, int
	*/
	//Scene* 
	int layerNum = luaL_checkint(L, 2);
	
}



int l_Layer_constructButton(lua_State* L){
	/*
		on stack should be Layer*,x,y,w,h, resid1, resid2, btnevt
	*/
	Layer* lay = (Layer*)luaL_checkudata(L, 1, "luaL_Layer");
	float x = luaL_checknumber(L, 2);
	float y = luaL_checknumber(L, 3);
	float w = luaL_checknumber(L, 4);
	float h = luaL_checknumber(L, 5);
	int resid1 = luaL_checkint(L, 6);
	int resid2 = luaL_checkint(L, 7);
	int btnevt = luaL_checkint(L, 8);
	lay->addButtonComponent(resid1, resid2, x, y, w, h, btnevt);
	return 0;
}
int l_Layer_constructTexture(lua_State* L){
	/*
	on stack should be Layer*,x,y,w,h, resid1
	*/
	Layer* lay = (Layer*)luaL_checkudata(L, 1, "luaL_Layer");
	float x = luaL_checknumber(L, 2);
	float y = luaL_checknumber(L, 3);
	float w = luaL_checknumber(L, 4);
	float h = luaL_checknumber(L, 5);
	int resid1 = luaL_checkint(L, 6);
	lay->addTextureComponent(resid1, x, y, w, h);
	return 0;
}

int l_Layer_constructWorld(lua_State* L){
	/*
	on stack should be Layer*
	*/
	Layer* lay = (Layer*)luaL_checkudata(L, 1, "luaL_Layer");
	lay->addGameWorld();
	return 0;
}





void LuaEngine::registerBindings(lua_State* state){
	luaL_Reg layerFunctions[] = {
		{ "constructButton", l_Layer_constructButton },
		{ "constructTexture", l_Layer_constructTexture },
		{ "constructWorld", l_Layer_constructWorld },
		{NULL,NULL}
	};
	luaL_newmetatable(state, "luaL_Layer");
	luaL_setfuncs(state, layerFunctions, 0);
	lua_pushvalue(state, -1);
	lua_setfield(state, -1, "__index");
	lua_setglobal(state, "Layer");
}