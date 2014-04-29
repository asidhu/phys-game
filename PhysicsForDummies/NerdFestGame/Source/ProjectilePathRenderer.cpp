#include "ProjectilePathRenderer.h"
#include "PhysGame\Source\RenderList.h"
#include "PhysicsEngine\Source\Body.h"
ProjectilePathRenderer::ProjectilePathRenderer(GameObject* m){
	target = m->m_obj;
	target->get();
	toff = 0;
}

bool ProjectilePathRenderer::tick(float t){
	if (!target->isAlive() || ! (*alive)){
		target->free();
		return true;
	}
	toff += t;
	if (toff >= .2f)
		toff -= .2f;
	return false;
}


void ProjectilePathRenderer::render(RenderList* list){
	if (!target->isAlive()) return;
	float astep = (af - ai) / num;
	float tstep = (tf - ti) / num;
	float aInitial = ai;
	float tInitial = ti + toff;
	body* obj = target->getdata()->getBody();
	float xi = obj->position.x, yi = obj->position.y, xv = obj->velocity.x, yv = obj->velocity.y;
	for (int i = 0; i < num; i++){
		RenderItem* itm = list->getItem();
		itm->myType = solidsquare;
		itm->x = xi + xv*tInitial;
		itm->y = yi + yv*tInitial + half_gravity*tInitial*tInitial;
		itm->rot = tInitial;
		itm->square.a = aInitial;
		itm->square.r = r;
		itm->square.g = g;
		itm->square.b = b;
		itm->square.w = 1.f;
		itm->square.h = 1.f;
		list->addItem(itm);
		tInitial += tstep;
		aInitial += astep;
	}
}