#pragma once
#include <vector>

class RenderList;
class Camera;
class LayerComponent{
public:
	virtual void render(RenderList*,Camera*, float x, float y)=0;
	virtual void mouseUpdate(float mX, float mY, float scrollX, float scrollY){}
	virtual bool mouseClick(int type, float mX, float mY, float scrollX, float scrollY){ return false; }
};
class Layer{
	bool scrolling;
	float x, y;
public:
	Layer();
	Layer(float w, float h);
	float width, height;
	bool enabled;
	std::vector<LayerComponent * > m_components;
	void render(RenderList*, Camera*);
	void addComponent(LayerComponent* comp);
	void addButtonComponent(int regularID, int mouseID, float x, float y, float w, float h, int btnEvent);
	void addTextureComponent(int regularID, float x, float y, float w, float h);
	void addGameWorld();
	void updateScroll(float offx, float offy,Camera* main_cam);
	void handleMouseUpdate(float x, float y);
	bool handleMouseClick(int type, float x, float y);

};