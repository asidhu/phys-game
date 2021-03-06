#include "Actor.h"
#include "PhysGame\Source\RenderList.h"
#include "PhysicsEngine\Source\Body.h"
#include "PhysicsEngine\Source\shape.h"
#include "Camera.h"

#define rad2deg(x) (x*57.2957795131) 

void Actor::render(RenderList* lst){
	if (m_body == NULL){
		RenderItem* item = lst->getItem();
		//FIX THIS USE ACCESSOR :OOOO
		item->x = m_body->position.x;
		item->y = m_body->position.y;
		if (m_body->form->type == BOX_TYPE)
		{
			item->myType = hollowsquare;
			item->square.a = item->square.r = item->square.g = item->square.b = 1.f;
			item->square.lw = 2;
			item->square.w = ((box*)m_body->form)->halfwidth * 2;
			item->square.h = ((box*)m_body->form)->halfheight * 2;
		}
		item->rot = m_body->rotation;
		lst->addItem(item);
	}
}


bool Actor::cullActor(Camera* cam){
	if (m_body == NULL)
	{
		return false;
	}
	else{
		rect r;
		r.left = cam->l;
		r.right = cam->r;
		r.top = cam->t;
		r.bottom = cam->b;
		return r.overlap(m_body->AABB);
	}
}