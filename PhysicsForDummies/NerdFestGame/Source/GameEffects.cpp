#include "GameEffects.h"
#include "PhysGame\Source\RenderList.h"
bool SmokeEffect::tick(float timestep){
	return (dur+=timestep)>SMOKELENGTH;
}

void SmokeEffect::render(RenderList* lst){
	RenderItem* itm = lst->getItem();
	itm->myType = solidcircle;
	itm->x = x;
	itm->y = y;
	itm->circle.radius =  (float)dur / SMOKELENGTH*3.f + .4f;
	itm->circle.a = .6f*(1.f- ((float)dur/SMOKELENGTH));
	itm->circle.r = itm->circle.g = itm->circle.b = .6f- ((float)dur / SMOKELENGTH*.4f);
	lst->addItem(itm);
}