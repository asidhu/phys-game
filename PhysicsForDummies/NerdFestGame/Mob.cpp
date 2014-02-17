#include "Mob.h"
#include "PhysGame\Source\RenderList.h"
#include "PhysicsEngine\Source\Body.h"
#include "PhysicsEngine\Source\shape.h"
#include "PhysicsEngine\Source\collision.h"

void check_post_collide(body* b, contactdetails* cd){
	Mob* m = (Mob*)b->data;
	Mob::checkExtremeCollision(m, cd);
}
Mob::Mob(int id, body* b) :GameObject(b,true,true){
	m_hp = 100;
	m_deathframes = 0;
	b->data = this;
	b->dataFlag |= MOB_FLAG;
	b->post_collide = check_post_collide;
}

void Mob::checkExtremeCollision(Mob* m, contactdetails* cd){
	if (cd->normalImpulse > 20){
		m->dmg((int)((cd->normalImpulse-20)/5));
	}
}

void Mob::render(RenderList* lst){
	RenderItem* item = lst->getItem();
	//FIX THIS USE ACCESSOR :OOOO
	//feed me a big fat penis
	body* b = getBody();
	item->x = b->position.x;
	item->y = b->position.y;
	item->myType = stenciltexture;
	item->tex.w = ((box*)b->form)->halfwidth * 2;
	item->tex.h = ((box*)b->form)->halfheight * 2;
	item->tex.s1 = item->y*.4f;
	item->tex.t1 = item->x*.4f;
	if (item->tex.h > item->tex.w){
		item->tex.s2 = item->y*.4f - item->tex.h * 2;
		item->tex.t2 = item->x*.4f - item->tex.h * 2;
	}
	else{
		item->tex.s2 = item->y*.4f - item->tex.w * 2;
		item->tex.t2 = item->x*.4f - item->tex.w * 2;
	}
	//item->tex.s1 = item->tex.t1 = 0;
	//item->tex.s2 = item->tex.t2 = 1;
	item->tex.resID = m_tex;
	item->rot = b->rotation;
	lst->addItem(item);

	if (dmgfx > 0 && (dmgfx % 8) > 4){
		RenderItem* item = lst->getItem();
		item->x = b->position.x;
		item->y = b->position.y;
		item->myType = solidsquare;
		item->rot = b->rotation;
		item->square.lw = 1;
		item->square.w = ((box*)b->form)->halfwidth * 2;
		item->square.h = ((box*)b->form)->halfheight * 2;
		item->square.a = 1;
		item->square.r = 1;
		item->square.g = item->square.b = 0;
		lst->addItem(item);
	}
	/*
	item = lst->getItem();
	item->x = b->position.x;
	item->y = b->position.y;
	item->myType = hollowsquare;
	item->rot = b->rotation;
	item->square.lw = 1;
	item->square.w = ((box*)b->form)->halfwidth * 2;
	item->square.h = ((box*)b->form)->halfheight * 2;
	item->square.a = 1;
	item->square.r = item->square.g = item->square.b = 1;
	lst->addItem(item);
	*/
}