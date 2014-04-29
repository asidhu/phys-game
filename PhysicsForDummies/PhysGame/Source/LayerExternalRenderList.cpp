#include "LayerExternalRenderList.h"
#include "Scene.h"
LayerExternalRenderList::LayerExternalRenderList(int id,Scene* s,bool follow){
	viewport = s->getCamera();
	m_ID = id;
	s->addExternalList(id,&myList);
	world = follow;
}

void LayerExternalRenderList::render(RenderList* list, Camera* bounds, float x, float y){
	if (world){
		Camera window;
		window.l = viewport->l + bounds->l - 10;
		window.t = viewport->t + bounds->t + 10;
		window.r = viewport->l + bounds->r + 10;
		window.b = viewport->t + bounds->b - 10;
		list->setupTransform(x, y, 1, 1);
	}
	//list->setupTransform(-viewport->l, -viewport->t, 1, 1);
	for (int i = 0; i < myList.getNumAllocatedItems(); i++){
		list->addItem(myList.getAllocatedItems().at(i));
	}
	myList.clear();
	if(world)
		list->resetTransform();

}