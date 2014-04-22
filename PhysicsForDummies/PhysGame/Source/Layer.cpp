#include "Layer.h"
#include "Camera.h"

void Layer::render(RenderList* list, Camera* cam){
	std::vector<LayerComponent*>::iterator start;
	for (start = m_components.begin(); start != m_components.end(); start++){
		LayerComponent* c = *start;
		c->render(list, cam, x, y);
	}
}