#include "LayerWorldViewport.h"
#include "Camera.h"
#include "GameWorld.h"
#include "RenderList.h"
#include "Scene.h"
LayerWorldViewport::LayerWorldViewport(Scene* s){
	viewport = s->getCamera();
}

void LayerWorldViewport::render(RenderList* list, Camera* bounds, float x, float y){
	Camera window;
	window.l = viewport->l + bounds->l-10;
	window.t = viewport->t + bounds->t+10;
	window.r = viewport->l + bounds->r+10;
	window.b = viewport->t + bounds->b-10;
//	list->setupTransform(x, y, 1, 1);
	list->setupTransform(-viewport->l, -viewport->t, 1, 1);
	world->render(list, &window, bounds);
	list->resetTransform();

}