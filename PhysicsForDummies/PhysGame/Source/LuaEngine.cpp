#include "LuaEngine.h"
#include <lua.hpp>
#include "Layer.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "GameEngine.h"
#include "Camera.h"
#include "ActorManager.h"
#include "GameWorld.h"
#include "Actor.h"
#include "PlatformDependent\PlatformGraphics.h"
/*
Lua Engine DOC

Objects:
Layer:
	constructButton x, y, w, h, regid, mouseoverid, btnevt
	constructTexture x, y, w, h, regid




*/


LuaEngine::LuaEngine(){
	m_state = 0L;
	m_hook = 0L;
}

void LuaEngine::initializeLua(Scene* s ){
	if (m_state != 0L){
		lua_getglobal(m_state, "end");
		lua_call(m_state, 0, 0);
		lua_close(m_state);
	}
	
	m_state = luaL_newstate();
	luaL_openlibs(m_state);
	registerBindings(m_state);
	Scene** udata = (Scene**)lua_newuserdata(m_state, sizeof(Scene*));
	*udata = s;
	luaL_getmetatable(m_state, "luaL_Scene");
	lua_setmetatable(m_state, -2);
	lua_setglobal(m_state, "Scene");

	if (m_hook){
		m_hook(m_state);
	}


	luaL_dofile(m_state, s->getLuaFile());
	lua_getglobal(m_state, "start");
	lua_call(m_state, 0, 0);
}

void LuaEngine::tick(float tstep){
	lua_getglobal(m_state, "update");
	lua_pushnumber(m_state, tstep);
	lua_call(m_state, 1, 0);
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

int l_PlatformGraphics_setupViewport(lua_State* L){
	// stack -> PlatformGraphics*, t,b,cx
	PlatformGraphics* graphics = *(PlatformGraphics**)lua_touserdata(L, 1); 
	float t = luaL_checknumber(L, 2),
		b = luaL_checknumber(L, 3),
		cx = luaL_checknumber(L, 4);
	graphics->setupViewport(t, b, cx);
	return 0;
}

int l_GameWorld_addActor(lua_State* L){
	// stack -> GameWorld*, Actor*
	GameWorld* gameWorld = *(GameWorld**)lua_touserdata(L, 1);
	Actor* myActor = *(Actor**)lua_touserdata(L, 2);
	gameWorld->addActor(myActor);
	return 0;
}


int l_ActorManager_getActorByID(lua_State* L){
	// stack -> actor manager*, id, w, h -> returns actor*
	ActorManager* actorManager = *(ActorManager**)lua_touserdata(L, 1);
	int id = luaL_checkint(L, 2);
	float w = luaL_checknumber(L, 3),
		h = luaL_checknumber(L, 4);
	Actor* myActor = actorManager->getActorByID(id,w,h);
	if (myActor == NULL){
		lua_pushnil(L);
	}
	else{
		Actor** actudata = (Actor**)lua_newuserdata(L, sizeof(Actor*));
		*actudata = myActor;
		luaL_getmetatable(L, "luaL_Actor");
		lua_setmetatable(L, -2);
	}
	return 1;
}

int l_ActorManager_getActorByType(lua_State* L){
	// stack -> actor manager*, id -> returns actor*
	ActorManager* actorManager = *(ActorManager**)lua_touserdata(L, 1);
	int id = luaL_checkint(L, 2);
	float w = luaL_checknumber(L, 3),
		h = luaL_checknumber(L, 4);
	Actor* myActor = actorManager->getActorByType(id, w, h);
	if (myActor == NULL){
		lua_pushnil(L);
	}
	else{
		Actor** actudata = (Actor**)lua_newuserdata(L, sizeof(Actor*));
		*actudata = myActor;
		luaL_getmetatable(L, "luaL_Actor");
		lua_setmetatable(L, -2);
	}
	return 1;
}

int l_ResourceManager_registerTexture(lua_State* L){
	// stack -> resmanager*, int, char*
	ResourceManager* resManager = *(ResourceManager**)lua_touserdata(L, 1);
	int resNumber = luaL_checkint(L, 2);
	const char* resName = luaL_checkstring(L, 3);
	resManager->registerTexture(resNumber, resName);
	return 0;
}


int l_Scene_getLayer(lua_State* L){
	/*
		stack should be a Scene*,int
		return layer
	*/
	Scene*  s = *(Scene**)lua_touserdata(L, 1);
	int layerNum = luaL_checkint(L, 2);
	Layer ** udata = (Layer **)lua_newuserdata(L, sizeof(Layer *));
	*udata = s->getLayer(layerNum);
	luaL_getmetatable(L, "luaL_Layer");
	lua_setmetatable(L, -2);
	return 1;
}

int l_Scene_getNumLayers(lua_State* L){
	/*
	stack should be a Scene*
	return layer
	*/
	Scene*  s = *(Scene**)lua_touserdata(L, 1);
	lua_pushinteger(L, s->getNumLayers());
	return 1;
}

int l_Scene_setSceneBounds(lua_State* L){
	/* stack should be Scene*, 4 numbers*/
	Scene*  s = *(Scene**)lua_touserdata(L, 1);
	float l = luaL_checknumber(L, 2),
		r = luaL_checknumber(L, 3),
		t = luaL_checknumber(L, 4),
		b = luaL_checknumber(L, 5);
	Camera* cam = s->getBounds();
	cam->l = l;
	cam->r = r;
	cam->t = t;
	cam->b = b;
	return 1;
}
int l_Scene_getSceneBounds(lua_State* L){
	/* stack should be Scene**/
	Scene*  s = *(Scene**)lua_touserdata(L, 1);
	Camera* cam = s->getBounds();
	lua_pushnumber(L, cam->l);
	lua_pushnumber(L, cam->r);
	lua_pushnumber(L, cam->t);
	lua_pushnumber(L, cam->b);
	return 4;
}
int l_Scene_getCameraBounds(lua_State* L){
	/* stack should be Scene**/
	Scene*  s = *(Scene**)lua_touserdata(L, 1);
	Camera* cam = s->getCamera();
	lua_pushnumber(L, cam->l);
	lua_pushnumber(L, cam->r);
	lua_pushnumber(L, cam->t);
	lua_pushnumber(L, cam->b);
	return 4;
}

int l_Scene_setCameraBounds(lua_State* L){
	/* stack should be Scene*, 4 numbers*/
	Scene*  s = *(Scene**)lua_touserdata(L, 1);
	float l = luaL_checknumber(L, 2),
		r = luaL_checknumber(L, 3),
		t = luaL_checknumber(L, 4),
		b = luaL_checknumber(L, 5);
	Camera* cam = s->getCamera();
	cam->l = l;
	cam->r = r;
	cam->t = t;
	cam->b = b;
	return 1;
}


int l_Layer_constructButton(lua_State* L){
	/*
		on stack should be Layer*,x,y,w,h, resid1, resid2, btnevt
	*/
	Layer* lay = *(Layer**)lua_touserdata(L, 1);
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
int l_Layer_setupDimensions(lua_State* L){
	// stack Layer*, w,h,
	Layer* lay = *(Layer**)lua_touserdata(L, 1);
	float w = luaL_checknumber(L, 2);
	float h = luaL_checknumber(L, 3);
	lay->width = w;
	lay->height = h;
	return 0;
}
int l_Layer_constructTexture(lua_State* L){
	/*
	on stack should be Layer*,x,y,w,h, resid1
	*/
	Layer* lay = *(Layer**)lua_touserdata(L, 1);
	float x = luaL_checknumber(L, 2);
	float y = luaL_checknumber(L, 3);
	float w = luaL_checknumber(L, 4);
	float h = luaL_checknumber(L, 5);
	int resid1 = luaL_checkint(L, 6);
	lay->addTextureComponent(resid1, x, y, w, h);
	return 0;
}

int l_Layer_setScrolling(lua_State* L){
	// stack Layer*, bool
	Layer* lay = *(Layer**)lua_touserdata(L, 1);
	bool scrolling = luaL_checkint(L, 2);
	lay->setScroll(scrolling);
	return 0;
}
int l_Layer_setEnabled(lua_State* L){
	// stack Layer*, bool
	Layer* lay = *(Layer**)lua_touserdata(L, 1);
	bool scrolling = luaL_checkint(L, 2);
	lay->enabled = scrolling;
	return 0;
}
int l_Layer_constructWorld(lua_State* L){
	/*
	on stack should be Layer*, Scene*
	*/
	Layer* lay = *(Layer**)lua_touserdata(L, 1);
	Scene*  s = *(Scene**)lua_touserdata(L, 2);
	lay->addGameWorld(s);
	return 0;
}

int l_Layer_constructExternalList(lua_State* L){
	/*
	on stack should be Layer*, Scene*, int id, bool 
	*/
	Layer* lay = *(Layer**)lua_touserdata(L, 1);
	Scene*  s = *(Scene**)lua_touserdata(L, 2);
	int id = luaL_checkint(L, 3);
	bool followWorld = luaL_checkint(L, 4);
	lay->addExternalList(id, followWorld, s);
	return 0;
}





void LuaEngine::sendButtonEvent(int evt){
	
	lua_getglobal(m_state, "evtButton");
	lua_pushinteger(m_state, evt);
	lua_call(m_state, 1, 0);


}





void LuaEngine::registerBindings(lua_State* state){
	luaL_Reg layerFunctions[] = {
		{ "constructButton", l_Layer_constructButton },
		{ "constructTexture", l_Layer_constructTexture },
		{ "constructWorld", l_Layer_constructWorld },
		{ "constructExternalList", l_Layer_constructExternalList },
		{ "setScrolling", l_Layer_setScrolling },
		{ "setEnabled", l_Layer_setEnabled },
		{ "setupDimensions", l_Layer_setupDimensions },
		{NULL,NULL}
	};
	luaL_newmetatable(state, "luaL_Layer");
	luaL_setfuncs(state, layerFunctions, 0);
	lua_pushvalue(state, -1);
	lua_setfield(state, -1, "__index");
	lua_setglobal(state, "Layer");

	luaL_Reg sceneFunctions[] = {
		{ "getLayer", l_Scene_getLayer },
		{ "getNumLayers", l_Scene_getNumLayers },
		{ "setSceneBounds", l_Scene_setSceneBounds },
		{ "setCameraBounds", l_Scene_setCameraBounds },
		{ "getSceneBounds", l_Scene_getSceneBounds },
		{ "getCameraBounds", l_Scene_getCameraBounds },
		{NULL,NULL}
	};
	luaL_newmetatable(state, "luaL_Scene");
	luaL_setfuncs(state, sceneFunctions, 0);
	lua_setfield(state, -1, "__index");

	luaL_Reg resManagerFunctions[] = {
		{ "registerTexture", l_ResourceManager_registerTexture },
		{NULL,NULL}
	};
	luaL_newmetatable(state, "luaL_ResourceManager");
	luaL_setfuncs(state, resManagerFunctions, 0);
	lua_pushvalue(state, -1);
	lua_setfield(state, -1, "__index");
	ResourceManager** manager = (ResourceManager**)lua_newuserdata(state, sizeof(ResourceManager*));
	*manager = engine->getResourceManager();
	luaL_getmetatable(state, "luaL_ResourceManager");
	lua_setmetatable(state, -2);
	lua_setglobal(state, "ResourceManager");

	luaL_Reg actorManagerFunctions[] = {
		{"getActorByID", l_ActorManager_getActorByID},
		{ "getActorByType", l_ActorManager_getActorByType },
		{NULL,NULL}
	};
	luaL_newmetatable(state, "luaL_ActorManager");
	luaL_setfuncs(state, actorManagerFunctions, 0);
	lua_pushvalue(state, -1);
	lua_setfield(state, -1, "__index");

	ActorManager** actormanager = (ActorManager**)lua_newuserdata(state, sizeof(ActorManager*));
	*actormanager = engine->getActorManager();
	luaL_getmetatable(state,"luaL_ActorManager");
	lua_setmetatable(state, -2);
	lua_setglobal(state, "ActorManager");


	luaL_Reg gameWorldFunctions[] = {
		{"addActor", l_GameWorld_addActor},
		{NULL,NULL}
	};
	luaL_newmetatable(state, "luaL_GameWorld");
	luaL_setfuncs(state, resManagerFunctions, 0);
	lua_pushvalue(state, -1);
	lua_setfield(state, -1, "__index");

	GameWorld** gameworld = (GameWorld**)lua_newuserdata(state, sizeof(GameWorld*));
	*gameworld = engine->getGameWorld();
	luaL_getmetatable(state, "luaL_GameWorld");
	lua_setmetatable(state, -2);
	lua_setglobal(state, "GameWorld");

	luaL_newmetatable(state, "luaL_Actor");
	lua_setfield(state, -1, "__index");

	luaL_Reg graphicsFunctions[] = {
		{"setupViewport",l_PlatformGraphics_setupViewport},
		{NULL,NULL}
	};
	luaL_newmetatable(state, "luaL_PlatformGraphics");
	luaL_setfuncs(state, graphicsFunctions, 0);
	lua_pushvalue(state, -1);
	lua_setfield(state, -1, "__index");

	PlatformGraphics** graphics = (PlatformGraphics**)lua_newuserdata(state, sizeof(PlatformGraphics*));
	*graphics = engine->getGraphics();
	luaL_getmetatable(state, "luaL_PlatformGraphics");
	lua_setmetatable(state, -2);
	lua_setglobal(state, "Graphics");

}

void LuaEngine::registerGlobals(lua_State* L){

}