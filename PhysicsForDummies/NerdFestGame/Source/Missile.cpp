#include "Missile.h"
#include "PhysGame\Source\GameEngine.h"
#include "Shrapnel.h"
#include "PhysicsEngine\Source\Body.h"
#include "PhysicsEngine\Source\collision.h"
#include "Mob.h"
#include "PhysicsEngine\Source\BodyDef.h"
#include "PhysicsEngine\Source\PhysEngine.h"
#include "PhysGame\Source\RenderList.h"
#include "GameEffects.h"
#include <cstdlib>
body* createBody9(PhysEngine* engine, float x, float y, float w, float h, float mass, float rot = 0){
	bodydef bdef;
	bdef.position.x = x;
	bdef.position.y = y;
	bdef.width = w;
	bdef.height = h;
	bdef.mass = mass;
	bdef.rotation = rot;
	return engine->buildBody(bdef);
}
bool Missile::tick(GameWorld* e){
	/*
	if (life == -1){
		int numGen = (int)(((float)rand()) / RAND_MAX * 4) + 2;
		for (int i = 0; i < numGen; i++)
		{
			float randX = ((float)rand()) / RAND_MAX, randY = ((float)rand()) / RAND_MAX;
			float randSize = ((float)rand()) / RAND_MAX * .3f + .3f;
			body* b = createBody9(e->getPhysEngine(), getBody()->position.x, getBody()->position.y, randSize, randSize, randSize / 80);
			b->velocity.x = randX*((float)rand()) / RAND_MAX * 10;
			b->velocity.y = randY*((float)rand()) / RAND_MAX * 10;
			Shrapnel* s = new Shrapnel(1, b);
			float rndColor = ((float)rand()) / RAND_MAX*.2f;
			s->r = 1 - ((float)rand()) / RAND_MAX*.2f;
			s->g = ((float)rand()) / RAND_MAX*.6f;
			s->b = .01f;
			s->life += (int)(((float)rand()) / RAND_MAX * 500)+200;
			e->addActor(s);
		}
		return true;
	}
	if (life %3==0)
		e->addEffect(new SmokeEffect(getBody()->position.x, getBody()->position.y));
	life++;
	*/
	return false;
}
void Missile::render(RenderList* lst){
	RenderItem* item = lst->getItem();
	body* b = getBody();
	//FIX THIS USE ACCESSOR :OOOO
	item->x = b->position.x;
	item->y = b->position.y;
	if (b->form->type == BOX_TYPE)
	{
		item->myType = solidsquare;
		item->square.a = item->square.r = item->square.g = item->square.b = 1.f;
		item->square.lw = 1;
		item->square.w = ((box*)b->form)->halfwidth * 2;
		item->square.h = ((box*)b->form)->halfheight * 2;
		item->square.r = .3f;
		item->square.g = .3f;
		item->square.b = .3f;

	}
	item->rot = b->rotation;
	lst->addItem(item);
}

bool missile_check_explode(body* b, contactdetails* cd){
	Missile* m = (Missile*)b->data;
	
	if (m->life > 6){
		return true;
	}
	body* other;
	if (cd->b1 == b)
		other = cd->b2;
	else
		other = cd->b1;
	return m->launcher->getBody() != other;
}
void missile_explode(body* b, contactdetails* cd){
	Missile* m = (Missile*)b->data;
	body* other;
	if (cd->b1 == b)
		other = cd->b2;
	else
		other = cd->b1;
	if (!(other->dataFlag&OKMISSILE))
		return;
	if (other->dataFlag& MOB_FLAG){
		Mob* mob = (Mob*)(other->data);
		mob->dmg(m->dmg);
	}
	m->life = -1;
}

Missile::Missile(int id, body* b) :GameObject(b,true,true){
	b->data = this;
	b->pre_collide = missile_check_explode;
	b->post_collide = missile_explode;
	dmg = 1;
	life = 0;
}