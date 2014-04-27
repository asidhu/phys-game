#include "LayerButton.h"
#include "Camera.h"
#include "RenderList.h"
#include "SceneManager.h"
#include "LuaEngine.h"

LayerButton::LayerButton(){
	mousedOver = false;
}
LayerButton::LayerButton(float x, float y, float w, float h, int texid, int overid, int btnevt){
	mX = x;
	mY = y;
	mW = w;
	mH = h;
	regularID = texid;
	mouseOverID = overid;
	mousedOver = false;
	buttonID = btnevt;
}
void LayerButton::render(RenderList* list,Camera* cam, float x, float y){
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
	if (!mousedOver)
		item->tex.resID = regularID;
	else
		item->tex.resID = mouseOverID;
	list->addItem(item);
}

void LayerButton::mouseUpdate(float mouseX, float mouseY, float scrollX, float scrollY){
	float dx = mouseX - mX - scrollX,
		dy = mouseY - mY - scrollY;
	if (abs(dx) < mW / 2 && abs(dy) < mH / 2){
		mousedOver = true;
	}
	else{
		mousedOver = false;
	}
}

bool LayerButton::mouseClick(SceneManager* sm, int type, float mouseX, float mouseY, float scrollX, float scrollY){
	float dx = mouseX - mX - scrollX,
		dy = mouseY - mY - scrollY;
	if (abs(dx) < mW / 2 && abs(dy) < mH / 2){
		sm->getLuaEngine()->sendButtonEvent(buttonID);
		return true;
	}
	return false;
}