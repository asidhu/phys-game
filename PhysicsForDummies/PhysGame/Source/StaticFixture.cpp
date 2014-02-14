#include "PhysGame\Source\StaticFixture.h"
#include "PhysGame\Source\RenderList.h"
#include "PhysicsEngine\Source\Body.h"
#include "PhysicsEngine\Source\shape.h"
void StaticFixture::render(RenderList* lst){
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