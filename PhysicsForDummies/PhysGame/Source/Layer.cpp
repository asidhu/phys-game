#include "Layer.h"
#include "Camera.h"


Layer::Layer(){
	this->x = this->y = 0;
	this->width = this->height = 0;
	scrolling = false;
}

Layer::Layer(float w, float h){
	x = y = 0;
	this->width = w;
	this->height = h;
	scrolling = true;
}

void Layer::render(RenderList* list, Camera* cam){
	std::vector<LayerComponent*>::iterator start;
	for (start = m_components.begin(); start != m_components.end(); start++){
		LayerComponent* c = *start;
		if (scrolling)
			c->render(list, cam, x, y);
		else
			c->render(list, cam, 0,0);
	}
}

void Layer::addComponent(LayerComponent* c){
	m_components.push_back(c);
}

void Layer::updateScroll(float offX, float offY, Camera* maincam){
	if (scrolling){
		float sX = maincam->w() / width,
			sY = maincam->h() / height;
		x = offX*sX;
		y = offY*sY;
	}
}