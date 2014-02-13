#pragma once
#include <list>
#include <vector>

class RenderItem{
public:
	float x, y;
	float a, r, g, b;
	float w, h;
	float rot;
};
class RenderList{
	std::vector<RenderItem*> allocateRenderItems;
	unsigned int size;
public:
	RenderList(){
		size = 0;
	}
	std::list<RenderItem*> renderItems;
	void addItem(RenderItem* i){
		renderItems.push_back(i);
	}

	RenderItem* getItem(){
		if (size >= allocateRenderItems.size())
		{
			allocateRenderItems.push_back(new RenderItem());
		}
		size++;
		RenderItem* ret= allocateRenderItems.back();
		allocateRenderItems.pop_back();
		return ret;
	}
	void clear(){
		renderItems.clear();
		size = 0;
	}

};