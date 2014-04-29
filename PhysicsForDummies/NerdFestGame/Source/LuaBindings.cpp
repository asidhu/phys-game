#include "LuaBindings.h"
#include "lua.hpp"
#include "PhysGame\Source\ActorManager.h"
#include "MyActorManager.h"
#include "PhysicsEngine\Source\PhysEngine.h"
#include "PhysGame\Source\GameEngine.h"
#include "PhysGame\Source\GameWorld.h"
#include "GameObjects.h"
#include "Wall.h"
void luaL_addfuncs(lua_State* L,const char* tname, luaL_Reg* funcs){
	luaL_getmetatable(L, tname);
	int i = 0;
	while (funcs[i].func){
		lua_pushcfunction(L, funcs[i].func);
		lua_setfield(L, -2, funcs[i].name);
		i++;
	}
	lua_pop(L,1);
}

body* lua_createbody(PhysEngine* engine, float x, float y, float w, float h, float mass, float rot = 0){
	bodydef bdef;
	bdef.position.x = x;
	bdef.position.y = y;
	bdef.width = w;
	bdef.height = h;
	bdef.mass = mass;
	bdef.rotation = rot;
	return engine->buildBody(bdef);
}

int l_ActorManager_constructWall(lua_State* L){
	// stack -> actormanager*, x, y, w, h, rot, texid
	MyActorManager* actorManager = *(MyActorManager**)lua_touserdata(L, 1);
	float x = luaL_checknumber(L, 2),
		y = luaL_checknumber(L, 3),
		w = luaL_checknumber(L, 4),
		h = luaL_checknumber(L, 5),
		rot = luaL_checknumber(L, 6);
	int texid = luaL_checkint(L, 7);
	PhysEngine* phys = actorManager->m_game_engine->getGameWorld()->m_physEngine;
	body* mybody=lua_createbody(phys, x, y, w, h, 0, rot);
	Wall* obj = actorManager->m_game_engine->getGameWorld()->allocateActor<Wall>(mybody, texid);
	actorManager->m_game_engine->getGameWorld()->addActor(obj);
	Actor** lua_obj = (Actor**)lua_newuserdata(L, sizeof(Actor*));
	*lua_obj = obj;
	luaL_setmetatable(L, "luaL_Actor");
	return 1;
}

int l_Actor_setXY(lua_State* L){
	//stack -> actor*, x, y
	Actor* act = *(Actor**)lua_touserdata(L, 1);
	float x = luaL_checknumber(L, 2),
		y = luaL_checknumber(L, 3);
	body* b = act->getBody();
	b->position.x = x;
	b->position.y = y;
	lua_pushvalue(L, 1);
	return 1;
}

int l_Actor_setMass(lua_State* L){
	//stack -> actor*, mass
	Actor* act = *(Actor**)lua_touserdata(L, 1);
	float m = luaL_checknumber(L, 2);
	body* b = act->getBody();
	b->setMass(m);
	lua_pushvalue(L, 1);
	return 1;
}


void registerGameBindings(lua_State* L){
	luaL_Reg new_ActorManagerFunctions[] = {
		{"constructWall",l_ActorManager_constructWall},
		{NULL,NULL}
	};
	luaL_addfuncs(L, "luaL_ActorManager", new_ActorManagerFunctions);

	luaL_Reg new_ActorFunctions[] = {
		{"setXY",l_Actor_setXY},
		{ "setMass", l_Actor_setMass },
		{NULL,NULL}
	};

	luaL_addfuncs(L, "luaL_Actor", new_ActorFunctions);


}