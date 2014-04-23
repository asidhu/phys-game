
#pragma once
#include "Layer.h"
#include "Camera.h"
class GameWorld;
class LayerWorldViewport : public LayerComponent{
public:
	Camera defaultVP;
	Camera *viewport;
	float mX, mY, mW, mH;
	GameWorld* world;
	void render(RenderList*, Camera*, float x, float y);
};