#include "GameButton.h"
#include "PhysicsEngine\Source\Body.h"
#include "PhysicsEngine\Source\collision.h"
#include "PhysGame\Source\RenderList.h"
#include "PhysicsEngine\Source\shape.h"
void GameButton::toggle(body* b, contactdetails* cd){
	GameButton *gb = (GameButton*)b->data;
	body* other;
	if (cd->b1 == b)
		other = cd->b2;
	else
		other = cd->b1;
	if (other->dataFlag&TRIGGERBUTTON)
		gb->isHit = gb->setTime;
}
GameButton::GameButton(body* b):GameObject(b,false,true){
	isHit = 0;
	setTime = 1;
	b->post_collide = GameButton::toggle;
}

bool GameButton::tick(float t, GameWorld* e){
	if (isHit > 0){
		isHit -= t;
	}
	return false;
}
bool GameButton::toggled(){
	return isHit > 0;
}
void GameButton::render(RenderList* lst){
	RenderItem* item = lst->getItem();
	//FIX THIS USE ACCESSOR :OOOO
	body* b = getBody();
	item->x = b->position.x;
	item->y = b->position.y;
	item->myType = stenciltexture;
	item->tex.w = ((box*)b->form)->halfwidth * 2;
	item->tex.h = ((box*)b->form)->halfheight * 2;
	item->tex.s1 = item->y*.4f;
	item->tex.t1 = item->x*.4f;
	item->zIndex = -1.f;
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
}