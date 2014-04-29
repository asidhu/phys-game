#include "Layer.h"
#include "Camera.h"
#include "LayerButton.h"
#include "LayerTexture.h"
#include "LayerWorldViewport.h"
#include "LayerExternalRenderList.h"
#include "Scene.h"
Layer::Layer(){
	this->x = this->y = 0;
	this->width = this->height = 0;
	scrolling = false;
	enabled = true;
}

Layer::Layer(float w, float h){
	x = y = 0;
	this->width = w;
	this->height = h;
	scrolling = true;
	enabled = true;
}

void Layer::addExternalList(int id, bool followWorld, Scene* s){
	LayerExternalRenderList* ext = new LayerExternalRenderList(id,s,followWorld);
	addComponent(ext);
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

void Layer::handleMouseUpdate(float x, float y){
	std::vector<LayerComponent*>::iterator it = m_components.begin();
	for (; it != m_components.end(); it++){
		LayerComponent* comp = *it;
		comp->mouseUpdate(x, y,this->x, this->y);
	}

}

bool Layer::handleMouseClick(SceneManager* m,int type, float x, float y){
	std::vector<LayerComponent*>::iterator it = m_components.begin();
	for (; it != m_components.end(); it	++){
		LayerComponent* comp = *it;
		if (comp->mouseClick(m,type,x, y, this->x, this->y))
			return true;
	}
	return false;

}

void Layer::addButtonComponent(int regularID, int mouseID, float x, float y, float w, float h, int btnEvent){
	LayerButton * btn = new LayerButton(x, y, w, h, regularID, mouseID,btnEvent);
	addComponent(btn);
}
void Layer::addTextureComponent(int regularID, float x, float y, float w, float h){
	LayerTexture * tex = new LayerTexture(x, y, w, h, regularID);
	addComponent(tex);
}
void Layer::addGameWorld(Scene* s){
	LayerWorldViewport* lwvp = new LayerWorldViewport(s);
	lwvp->world = s->getGameWorld();
	addComponent(lwvp);
}