#include "LayerButton.h"
#include "Camera.h"
#include "RenderList.h"

LayerButton::LayerButton(){
	mousedOver = false;
}
void LayerButton::render(RenderList* list,Camera* cam, float x, float y){
	float displayX = cam->l + mX - x,
		displayY = cam->t + mY - y;
	RenderItem* item = list->getItem();
	item->myType = DrawType::texture;
	item->x = displayX;
	item->y = displayY;
	item->tex.w = mW;
	item->tex.h = mH;
	item->rot = 0;
	item->tex.s1 = 1;
	item->tex.s2 = 1;
	if (!mousedOver)
		item->tex.resID = regularID;
	else
		item->tex.resID = mouseOverID;
	list->addItem(item);
}