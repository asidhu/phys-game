#pragma once
#include <list>
#include <vector>
#include <queue>
#include "Camera.h"
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
	float zIndex;
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
		zIndex = 0;
		
	}
};
class RenderList{
	std::vector<RenderItem*> allocateRenderItems;
	unsigned int size;
	float tX, tY, sX, sY;
public:
	RenderList(){
		size = 0;
		tX = 0;
		tY = 0;
		sX = 1;
		sY = 1;
	}
	std::list<RenderItem*> renderItems;
	std::list<RenderItem*> batchFillCircle, batchFillSquare;
	std::list<RenderItem*> batchDrawCircle, batchDrawSquare;
	
	std::priority_queue<RenderItem*> batchTexSquare;

	Camera* viewport, *window;
	
	void setupTransform(float x, float y, float sx, float sy){
		tX = x;
		tY = y;
		sX = sx;
		sY = sy;
	}
	RenderItem* transformRenderItem(RenderItem* item){
		item->x += tX;
		item->y += tY;
		if (item->myType == solidcircle || item->myType == hollowcircle){
			item->circle.radius *= sX;
		}
		else if (item->myType == solidsquare || item->myType == hollowsquare){
			item->square.w *= sX;
			item->square.h *= sY;
		}
		else if (item->myType == texture || item->myType == stenciltexture){
			item->tex.w *= sX;
			item->tex.h *= sY;
		}
		return item;
	}

	void addItem(RenderItem* i){
		transformRenderItem(i);
		if (i->myType == solidcircle)
			batchFillCircle.push_back(i);
		else if (i->myType == solidsquare)
			batchFillSquare.push_back(i);
		else if (i->myType == hollowcircle)
			batchDrawCircle.push_back(i);
		else if (i->myType == hollowsquare)
			batchDrawSquare.push_back(i);
		else if (i->myType == texture || i->myType == stenciltexture)
			batchTexSquare.push(i);
		else
			renderItems.push_back(i);
	}

	void resetTransform(){
		tX = tY = 0;
		sX = sY = 1.f;
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
		batchDrawCircle.clear();
		batchDrawSquare.clear();
		batchTexSquare.empty();
		renderItems.clear();
		size = 0;
	}

};