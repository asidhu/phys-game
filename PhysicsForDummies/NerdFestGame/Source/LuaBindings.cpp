#include "LuaBindings.h"
#include "lua.hpp"
#include "PhysGame\Source\ActorManager.h"
#include "MyActorManager.h"
#include "PhysicsEngine\Source\PhysEngine.h"
#include "PhysGame\Source\GameEngine.h"
#include "PhysGame\Source\GameWorld.h"
#include "GameButton.h"
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
	mybody->linearDamping = 1;
	mybody->coeffFriction = 1;
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

int l_Actor_getXY(lua_State* L){
	//stack -> actor*,
	Actor* act = *(Actor**)lua_touserdata(L, 1);
	body* b = act->getBody();
	lua_pushnumber(L,b->position.x);
	lua_pushnumber(L,b->position.y);
	return 2;
}

int l_Actor_setRot(lua_State* L){
	//stack -> actor*, x
	Actor* act = *(Actor**)lua_touserdata(L, 1);
	float x = luaL_checknumber(L, 2);
	body* b = act->getBody();
	b->rotation = x;
	lua_pushvalue(L, 1);
	return 1;
}

int l_Actor_getRot(lua_State* L){
	//stack -> actor*,
	Actor* act = *(Actor**)lua_touserdata(L, 1);
	body* b = act->getBody();
	lua_pushnumber(L, b->rotation);
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
int l_Actor_setTexture(lua_State* L){
	//stack -> actor*, texid
	GameObject* act = (GameObject*)*(Actor**)lua_touserdata(L, 1);
	int txid = luaL_checkint(L, 2);
	act->giveInfo(INFOTYPE_RESID, txid);
	lua_pushvalue(L, 1);
	return 1;
}

int l_Actor_lockRotation(lua_State* L){
	//stack -> actor*, 
	GameObject* act = (GameObject*)*(Actor**)lua_touserdata(L, 1);
	act->getBody()->lockRotation();
	lua_pushvalue(L, 1);
	return 1;
}
int l_Actor_setV(lua_State* L){
	//stack -> actor*, x, y
	Actor* act = *(Actor**)lua_touserdata(L, 1);
	float x = luaL_checknumber(L, 2),
		y = luaL_checknumber(L, 3);
	body* b = act->getBody();
	b->velocity.x = x;
	b->velocity.y = y;
	lua_pushvalue(L, 1);
	return 1;
}
int l_Actor_addImpulse(lua_State* L){
	//stack -> actor*, x, y
	Actor* act = *(Actor**)lua_touserdata(L, 1);
	float x = luaL_checknumber(L, 2),
		y = luaL_checknumber(L, 3);
	body* b = act->getBody();
	b->impulse.x += x;
	b->impulse.y += y;
	lua_pushvalue(L, 1);
	return 1;
}
int l_Actor_getV(lua_State* L){
	//stack -> actor*,
	Actor* act = *(Actor**)lua_touserdata(L, 1);
	body* b = act->getBody();
	lua_pushnumber(L, b->velocity.x);
	lua_pushnumber(L, b->velocity.y);
	return 2;
}
int l_Actor_isToggled(lua_State* L){
	//stack -> GameButton,
	GameButton* act = *(GameButton**)lua_touserdata(L, 1);
	body* b = act->getBody();
	lua_pushboolean(L, act->toggled());
	return 1;
}
int l_Actor_setBtnTime(lua_State* L){
	//stack -> GameButton,
	GameButton* act = *(GameButton**)lua_touserdata(L, 1);
	body* b = act->getBody();
	float time = luaL_checknumber(L, 2);
	act->setTime = time;
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
		{ "setXY", l_Actor_setXY },
		{ "getXY", l_Actor_getXY },
		{ "setRot", l_Actor_setRot },
		{ "getRot", l_Actor_getRot },
		{ "setMass", l_Actor_setMass },
		{ "setTexture", l_Actor_setTexture },
		{ "lockRotation", l_Actor_lockRotation },
		{ "setV", l_Actor_setV },
		{ "getV", l_Actor_getV },
		{ "addImpulse", l_Actor_addImpulse},
		{ "isToggled", l_Actor_isToggled },
		{ "setButtonTime", l_Actor_setBtnTime },
		{NULL,NULL}
	};

	luaL_addfuncs(L, "luaL_Actor", new_ActorFunctions);


}