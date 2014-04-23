#pragma once
#include <vector>
#include "Camera.h"
class RenderList;
class StaticFixture;
class PlatformGraphics;
class Actor;
class Layer;
class Scene{
private:
	RenderList* m_lists;
	Layer* m_layers;
	int m_numlayers;
	Camera m_Bounds;
	Camera m_mainCam;
public:
	Scene(int numLayers);
	Camera* getCamera(){ return &m_mainCam; }
	Camera* getBounds(){ return &m_Bounds; }
	RenderList* render(int, Camera*);
	virtual void tick();
	int getNumLayers(){
		return m_numlayers;
	}
	Layer* getLayer(int layer);
};
