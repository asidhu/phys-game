#include "GameObjects.h"
#include "Grapple.h"
#include "PhysicsEngine\Source\Body.h"
#include "PhysGame\Source\RenderList.h"
GameObject::GameObject(body* b, bool grapple, bool missile):Actor(1,b){
	m_obj = new autoptr<GameObject>(this);
	m_obj->get();
	b->data = this;
	if (grapple)
		b->dataFlag |= OKGRAPPLE;
	if (missile)
		b->dataFlag |= OKMISSILE;
	b->dataFlag |= OKGRAVITYWELL;
	m_grapple = NULL;
	renderPathDetails = NULL;
}
void GameObject::grapple(Grapple* m){
	m_grapple = m;
}
void GameObject::releaseHook(){
	m_grapple = NULL;
}
void GameObject::release(){
	m_obj->kill();
	m_obj->free();
}

void GameObject::setAffectedByGravityWell(bool g){
	if (g)
		getBody()->dataFlag |= OKGRAVITYWELL;
	else
		getBody()->dataFlag &= ~OKGRAVITYWELL;
}
void GameObject::renderPath(RenderList* list, body* obj){
	if (!(*renderPathDetails->alive))return;
	float astep = (renderPathDetails->af - renderPathDetails->ai) / renderPathDetails->num;
	float tstep = (renderPathDetails->tf - renderPathDetails->ti) / renderPathDetails->num;
	float aInitial = renderPathDetails->ai;
	float tInitial = renderPathDetails->ti + renderPathDetails->toff;
	float xi = obj->position.x, yi = obj->position.y, xv = obj->velocity.x, yv = obj->velocity.y;
	for (int i = 0; i < renderPathDetails->num; i++){
		RenderItem* itm = list->getItem();
		itm->myType = solidsquare;
		itm->x = xi + xv*tInitial;
		itm->y = yi + yv*tInitial + renderPathDetails->half_gravity*tInitial*tInitial;
		itm->rot = tInitial;
		itm->square.a = aInitial;
		itm->square.r = renderPathDetails->r;
		itm->square.g = renderPathDetails->g;
		itm->square.b = renderPathDetails->b;
		itm->square.w = 1.f;
		itm->square.h = 1.f;
		list->addItem(itm);
		tInitial += tstep;
		aInitial += astep;
	}
}