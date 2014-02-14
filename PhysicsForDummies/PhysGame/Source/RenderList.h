#pragma once
#include <list>
#include <vector>

enum DrawType{
	solidsquare, hollowsquare,
	solidcircle, hollowcircle,
	texture, stenciltexture
};

class RenderItem{
public:
	float x, y;
	float rot;
	DrawType myType;
	union{
		struct{
			float a, r, g, b, w, h,lw;
		}square;
		struct{
			float a, r, g, b, radius,lw;
		}circle;
		struct{
			float w,h,s1, t1, s2, t2;
			unsigned int resID;
		}tex;
	};
	RenderItem(){
		myType = hollowsquare;
		
	}
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