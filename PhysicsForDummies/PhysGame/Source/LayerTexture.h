#include "Layer.h"

class LayerTexture : public LayerComponent{
	int regularID;
	float mX, mY, mW, mH;
public:
	LayerTexture(float x, float y, float w, float h, int id);
	void render(RenderList*, Camera*, float x, float y);
};