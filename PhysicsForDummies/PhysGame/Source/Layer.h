#include <vector>

class RenderList;
class Camera;
class LayerComponent{
public:
	virtual void render(RenderList*,Camera*, float x, float y)=0;
};
class Layer{
	float x, y;
	float width, height;
	std::vector<LayerComponent * > m_components;
public:
	void render(RenderList*, Camera*);


};