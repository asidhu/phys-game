#pragma once
#include "GameObjects.h"
#include "PhysicsEngine\Source\math.h"

class Player;
class Grapple: public GameObject{
public:
	Player* player;
	autoptr<GameObject> *attach, *otherattach;
	body* otherHook;

	vec2 relativeAttach, otherRelAttach;
	float force;
	bool shouldreturn;
	bool destroy;
	float maxlife;
	float lifetime;
	float maxdist;
	float speed;
	Grapple(int id, body* b);
	bool tick(float timestep, GameWorld*);
	void erase(GameWorld*);
	void createOtherHook(body*);
	bool oneHookOut(float t,GameWorld*);
	bool twoHookOut(float t,GameWorld*);
	void render(RenderList*);
	void release();
};