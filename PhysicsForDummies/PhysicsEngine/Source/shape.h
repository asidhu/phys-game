#pragma once
#include "math.h"
class rect;
class body;
class contactdetails;
#define CIRCLE_TYPE 1
#define BOX_TYPE 2
class shape{
public:
	int type;
	virtual void calcAABB(body* b, rect* aabb)= 0;
};

class circle : public shape{
public:
	circle(){
		type = CIRCLE_TYPE;
	}
	float radius;
	void calcAABB(body* b, rect* aabb);
};


struct extent{
	float min, max;
	unsigned char min_point, max_point;
};

class box : public shape{
public:
	box(){
		type = BOX_TYPE;
	}
	vec2 a, b, c, d;
	float halfwidth, halfheight;
	void calcAABB(body* b,rect* aabb);
	void getProjections(vec2& relPos, vec2& axis1, vec2& axis2, extent& axis1_span, extent& axis2_span);
	void getProjectionsXY(vec2& relPos, float& minX, float& maxX, float& minY, float& maxY);
	void calcOBB(float rot);
};
