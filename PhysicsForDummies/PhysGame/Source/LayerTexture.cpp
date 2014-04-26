#include "LayerTexture.h"
#include "Camera.h"
#include "RenderList.h"

LayerTexture::LayerTexture(float x, float y, float w, float h, int id){
	mX = x;
	mY = y;
	mW = w;
	mH = h;
	regularID = id;
}
void LayerTexture::render(RenderList* list, Camera* cam, float x, float y){
	float displayX = cam->l + mX + x,
		displayY = cam->t - mY + y;
	RenderItem* item = list->getItem();
	item->myType = DrawType::texture;
	item->x = displayX;
	item->y = displayY;
	item->tex.w = mW;
	item->tex.h = mH;
	item->rot = 0;
	item->tex.s1 = 1;
	item->tex.s2 = 1;
	item->tex.resID = regularID;
	list->addItem(item);
}