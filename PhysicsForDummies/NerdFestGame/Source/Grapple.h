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
	int maxlife;
	int lifetime;
	float maxdist;
	float speed;
	Grapple(int id, body* b);
	bool tick(GameEngine*);
	void erase(GameEngine*);
	void createOtherHook(body*);
	bool oneHookOut(GameEngine*);
	bool twoHookOut(GameEngine*);
	void render(RenderList*);
	void release();
};