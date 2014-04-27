
#include "Layer.h"
class Camera;
class LayerButton : public LayerComponent{
	int regularID;
	int mouseOverID;
	int buttonID;
	float mX, mY, mW, mH;
	bool mousedOver;
public:
	LayerButton();
	LayerButton(float x, float y, float w, float h, int texid, int overid,int btnevt);
	void mouseUpdate(float mX,float mY, float scrollX, float scrollY);
	bool mouseClick(SceneManager*,int type, float mX, float mY, float sX, float sY);
	void render(RenderList*,Camera*, float x, float y);
};