#pragma once

#include "Layer.h"
#include "RenderList.h"
class Camera;
class LayerExternalRenderList : public LayerComponent{
	RenderList myList;
	bool world;
	Camera * viewport;
	int m_ID;
public:
	LayerExternalRenderList(int list_ID,Scene*,bool followorld);
	void mouseUpdate(float mX, float mY, float scrollX, float scrollY){}
	bool mouseClick(SceneManager*, int type, float mX, float mY, float sX, float sY){ return false; }
	void render(RenderList*, Camera*, float x, float y);
};