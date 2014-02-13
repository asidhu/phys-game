#include "Actor.h"
#include "PhysGame\Source\RenderList.h"
#include "PhysicsEngine\Source\Body.h"
#include "PhysicsEngine\Source\shape.h"


#define rad2deg(x) (x*57.2957795131) 

void Actor::render(RenderList* lst){
	RenderItem* item = lst->getItem();
	//FIX THIS USE ACCESSOR :OOOO
	item->x = m_body->position.x;
	item->y = m_body->position.y; 
	if (m_body->form->type == BOX_TYPE)
	{
		item->w = ((box*)m_body->form)->halfwidth * 2;
		item->h = ((box*)m_body->form)->halfheight * 2;
	}
	item->rot = m_body->rotation;
	lst->addItem(item);
}