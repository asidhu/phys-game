#pragma once
#include <vector>

class RenderList;
class Camera;
class LayerComponent{
public:
	virtual void render(RenderList*,Camera*, float x, float y)=0;
};
class Layer{
	bool scrolling;
	float x, y;
public:
	Layer();
	Layer(float w, float h);
	float width, height;
	std::vector<LayerComponent * > m_components;
	void render(RenderList*, Camera*);
	void addComponent(LayerComponent* comp);
	void updateScroll(float offx, float offy,Camera* main_cam);
};