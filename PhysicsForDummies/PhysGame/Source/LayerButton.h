
#include "Layer.h"
class Camera;
class LayerButton : public LayerComponent{
	int regularID;
	int mouseOverID;
	float mX, mY, mW, mH;
	bool mousedOver;
public:
	LayerButton();
	void render(RenderList*,Camera*, float x, float y);
};