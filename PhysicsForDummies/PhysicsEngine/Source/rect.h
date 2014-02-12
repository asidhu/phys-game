#pragma once

class rect{
public:
	rect(){
		left = right = top = bottom = 0;
	}
	float left, right, top, bottom;
	float width(){ return right - left;}
	float height(){ return bottom - top; }
	bool overlap(rect& r){
		if (r.right <left || r.left > right || r.top<bottom || r.bottom>top)
			return false;
		return true;
	}
};