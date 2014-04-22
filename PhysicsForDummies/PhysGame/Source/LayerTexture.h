#include "Layer.h"

class LayerTexture : public LayerComponent{
	int regularID;
	float mX, mY, mW, mH;
public:
	void render(RenderList*, Camera*, float x, float y);
};