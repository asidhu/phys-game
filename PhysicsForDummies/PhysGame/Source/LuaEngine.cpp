#include "LuaEngine.h"
#include <lua.hpp>


LuaEngine::LuaEngine(){
	m_state = 0L;
}

void LuaEngine::initializeLua(){
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







void LuaEngine::registerBindings(lua_State* state){

}