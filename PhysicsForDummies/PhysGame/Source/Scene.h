#pragma once
#include <vector>
class RenderList;
class StaticFixture;
class PlatformGraphics;
class Actor;
class Layer;
class Camera;
class Scene{
private:
	RenderList* m_lists;
	Layer* m_layers;
	int m_numlayers;
public:
	Scene(int numLayers);
	RenderList* render(int, Camera*);
	virtual void tick();
};
