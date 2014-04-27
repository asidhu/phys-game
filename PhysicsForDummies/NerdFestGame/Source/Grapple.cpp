#include "Grapple.h"
#include "PhysicsEngine\Source\Body.h"
#include "PhysGame\Source\RenderList.h"
#include "PhysicsEngine\Source\shape.h"
#include "PhysGame\Source\GameEngine.h"
#include "PhysicsEngine\Source\PhysEngine.h"
#include "Player.h"
#include "Missile.h"



bool grapplehooknothit(body* b, contactdetails* cd){
	Grapple* hook = (Grapple*)b->data;
	body* other;
	if (cd->b1 == b) other = cd->b2;
	else other = cd->b1;
	if (!(other->dataFlag&OKGRAPPLE))
		return false;
	if (hook->player->getBody() == cd->b1 || hook->player->getBody() == cd->b2)
		return false;
	if (hook->otherHook == b)
		return hook->otherattach == NULL;
	if (hook->attach != NULL || hook->shouldreturn)
		return false;
	return true;
}

void grapplehookhit(body* b, contactdetails* cd){
	Grapple* hook = (Grapple*)b->data;
	if (hook->otherHook == b){
		if (hook->otherattach != NULL)
			return;
		if (b == cd->b1)
			hook->otherattach = ((GameObject*)cd->b2->data)->m_obj;
		else
			hook->otherattach = ((GameObject*)cd->b1->data)->m_obj;
		if (hook->otherattach == hook->attach)
		{
			hook->otherattach = NULL;
			return;
		}
		GameObject* grap = (GameObject*)hook->otherattach->getdata();
		hook->otherattach->get();
		hook->otherRelAttach = hook->otherHook->position - grap->getBody()->position;
	}
	else{
		if (hook->attach != NULL)
			return;
		if (b == cd->b1)
			hook->attach = ((GameObject*)cd->b2->data)->m_obj;
		else
			hook->attach = ((GameObject*)cd->b1->data)->m_obj;
		GameObject* grap = (GameObject*)hook->attach->getdata();
		hook->attach->get();
		hook->relativeAttach = hook->getBody()->position - grap->getBody()->position;
		hook->getBody()->velocity.x = hook->getBody()->velocity.y = 0;
	}

}
Grapple::Grapple(int id, body* b) :GameObject(b,false,false){
	attach = otherattach=NULL;
	shouldreturn = false;
	destroy = false;
	maxlife = 150;
	maxdist = 500 * 500;
	lifetime = 600;
	otherHook = NULL;
	b->data = this;
	b->pre_collide = grapplehooknothit;
	b->post_collide = grapplehookhit;
}

void Grapple::erase(GameWorld* e){
	if(player!=NULL && player->grappleHook==this)
		player->grappleHook = NULL;
	if (attach != NULL)
		attach->free();
	if (otherattach != NULL)
		otherattach->free();
	//if (otherHook != NULL)
	//	e->getPhysEngine()->removeBody(otherHook);
}

void Grapple::createOtherHook(body* b){
	otherHook = b;
	b->data = this;
	b->pre_collide = grapplehooknothit;
	b->post_collide = grapplehookhit;
}

bool Grapple::oneHookOut(GameWorld* e){
	vec2 dist = player->getBody()->position - getBody()->position;
	float d = dist.lengthSq();
	if (d > maxdist)
		shouldreturn = true;
	if (shouldreturn){
		dist.normalize();
		//dist *= force;
		//player->getBody()->applyImpulse(-1 * dist);
		//getBody()->applyImpulse(dist);
		dist *= speed+player->getBody()->velocity.length();
		getBody()->velocity = dist;
		if (destroy || d < 2)
		{
			erase(e);
			return true;
		}
		return false;
	}
	if (attach == NULL){
		if (maxlife-- < 0)
			shouldreturn = true;
		if (destroy)
		{
			erase(e);
			return true;
		}
		return false;
	}
	getBody()->angularVelocity = 1.5f;
	this->getBody()->position = attach->getdata()->getBody()->position + relativeAttach;
	dist.normalize();
	dist *= force;
	player->getBody()->applyImpulse(-1 * dist);
	attach->getdata()->getBody()->applyImpulse(dist);
	if (lifetime-- < 0)
		shouldreturn = true;
	
	return false;
}
bool Grapple::twoHookOut(GameWorld* e){
	if (attach != NULL){
		getBody()->angularVelocity = 1.5f;
		this->getBody()->position = attach->getdata()->getBody()->position + relativeAttach;
	}
	if (otherattach != NULL){
		otherHook->angularVelocity = 1.5f;
		otherHook->position = otherattach->getdata()->getBody()->position + otherRelAttach;
	}
	if (lifetime-- < 0){
		player = NULL;
		erase(e);
		return true;
	}
	if (attach != NULL && otherattach != NULL){
		vec2 dist = getBody()->position - otherHook->position;
		if (dist.lengthSq() > maxdist)
		{
			player = NULL;
			erase(e);
			return true;
		}
		dist.normalize();
		dist *= force;
		attach->getdata()->getBody()->applyImpulse(-1 * dist);
		otherattach->getdata()->getBody()->applyImpulse(dist);
	}
	return false;
}
bool Grapple::tick(float timestep, GameWorld* e){
	if (attach != NULL && !attach->isAlive())
		destroy = true;
	if (otherattach != NULL && !otherattach->isAlive())
		destroy = true;
	if (destroy){
		erase(e);
		return true;
	}
	if (otherHook == NULL)
		return oneHookOut(e);
	return twoHookOut(e);
}
void Grapple::release(){
	destroy = true;
}

void Grapple::render(RenderList* lst){
	RenderItem* item = lst->getItem();
	//FIX THIS USE ACCESSOR :OOOO
	item->x = getBody()->position.x;
	item->y = getBody()->position.y;
	if (getBody()->form->type == BOX_TYPE)
	{
		item->myType = solidsquare;
		item->square.a = item->square.r = item->square.g = item->square.b = 1.f;
		item->square.lw = 2;
		item->square.w = ((box*)getBody()->form)->halfwidth * 2;
		item->square.h = ((box*)getBody()->form)->halfheight * 2;
	}
	item->rot = getBody()->rotation;
	lst->addItem(item);
	if (otherHook != NULL){
		item = lst->getItem();
		//FIX THIS USE ACCESSOR :OOOO
		item->x = otherHook->position.x;
		item->y = otherHook->position.y;
		if (otherHook->form->type == BOX_TYPE)
		{
			item->myType = solidsquare;
			item->square.a = item->square.r = item->square.g = item->square.b = 1.f;
			item->square.lw = 2;
			item->square.w = ((box*)otherHook->form)->halfwidth * 2;
			item->square.h = ((box*)otherHook->form)->halfheight * 2;
		}
		item->rot = otherHook->rotation;
		lst->addItem(item);
	}
}