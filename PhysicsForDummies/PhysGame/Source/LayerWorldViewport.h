
#pragma once
#include "Layer.h"
#include "Camera.h"
class GameWorld;
class Scene;
class LayerWorldViewport : public LayerComponent{
public:
	Camera defaultVP;
	Camera *viewport;
	float mX, mY, mW, mH;
	GameWorld* world;
	LayerWorldViewport(Scene*);
	void render(RenderList*, Camera*, float x, float y);
};