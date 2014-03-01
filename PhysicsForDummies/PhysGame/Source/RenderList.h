#pragma once
#include <list>
#include <vector>
#include <queue>
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
	std::list<RenderItem*> batchFillCircle, batchFillSquare;
	std::priority_queue<RenderItem*> batchTexSquare;
	void addItem(RenderItem* i){
		if (i->myType == solidcircle)
			batchFillCircle.push_back(i);
		else if (i->myType == solidsquare)
			batchFillSquare.push_back(i);
		else if (i->myType == texture || i->myType == stenciltexture)
			batchTexSquare.push(i);
		else
			renderItems.push_back(i);
	}

	RenderItem* getItem(){
		if (size >= allocateRenderItems.size())
		{
			allocateRenderItems.push_back(new RenderItem());
		}
		RenderItem* ret = allocateRenderItems.at(size);
		size++;
		return ret;
	}
	void clear(){
		batchFillSquare.clear();
		batchFillCircle.clear();
		batchTexSquare.empty();
		renderItems.clear();
		size = 0;
	}

};