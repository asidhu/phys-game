#include "Shrapnel.h"
#include "PhysicsEngine\Source\Body.h"
#include "PhysicsEngine\Source\collision.h"
#include "Mob.h"
#include "PhysicsEngine\Source\BodyDef.h"
#include "PhysicsEngine\Source\shape.h"
#include "Missile.h"
#include "Grapple.h"
#include "PhysGame\Source\RenderList.h"
bool Shrapnel::tick(GameEngine* e){
	if (life--<0){
		return true;
	}
	return false;
}

Shrapnel::Shrapnel(int id, body* b) :Actor(id, b){
	b->data = this;
	life = 300;
}

void Shrapnel::render(RenderList* lst){
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
		item->square.r = r;
		item->square.g = g;	
		item->square.b = this->b;
		
	}
	item->rot = b->rotation;
	lst->addItem(item);
}