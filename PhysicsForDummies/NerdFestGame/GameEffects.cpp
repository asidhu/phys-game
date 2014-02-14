#include "GameEffects.h"
#include "PhysGame\Source\RenderList.h"
bool SmokeEffect::tick(){
	return dur++>SMOKELENGTH;
}

void SmokeEffect::render(RenderList* lst){
	RenderItem* itm = lst->getItem();
	itm->myType = solidcircle;
	itm->x = x;
	itm->y = y;
	itm->circle.radius = (float)dur / SMOKELENGTH*1.f + .2f;
	itm->circle.a = 1;
	itm->circle.r = itm->circle.g = itm->circle.b = .6f- ((float)dur / SMOKELENGTH*.4f);
	lst->addItem(itm);
}