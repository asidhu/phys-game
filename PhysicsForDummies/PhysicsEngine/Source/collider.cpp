
#include "collider.h"
#include "shape.h"
#include "Body.h"
#include "collision.h"
#define max(a,b) ((a>b)?a:b)
#define min(a,b) ((a<b)?a:b)


inline bool overlap(const collider::extent& a, const collider::extent& b){
	return !(a.max<b.min || a.min>b.max);
}
inline bool overlap(const collider::extent& a, const float min, const float max){
	return !(a.max<min || a.min>max);
}
inline float halfextent(const extent& a){
	return (a.max - a.min) / 2;
}
inline vec2 extent2point(const unsigned char a, const box* b){
	int num = 4;
	vec2 avg;
	if ((a & 1) != 0)
		avg += b->a;
	else num--;

	if ((a & 2) != 0)
		avg += b->b;
	else num--;

	if ((a & 4) != 0)
		avg += b->c;
	else num--;

	if ((a & 8) != 0)
		avg += b->d;
	else num--;
	avg *= 1.0f / num;
	return avg;
}
inline float penetration(const collider::extent& a, const collider::extent& b){
	float diff1 = abs(a.min - b.max);
	float diff2 = abs(a.max - b.min);
	return min(diff1, diff2);
}
inline bool floatEqualToBias(const float a, const float b){
	const float bias = .0001f;
	return fabs(a - b) < bias;
}
inline bool floatLessThanBias(const float a, const float b){
	const float bias = .0001f;
	return a-bias<b;
}




bool collider::collideCircleCircle(body* a, body* b, contactdetails* cd){
	circle* c1 = (circle*)(a->form), *c2 = (circle*)(b->form);
	vec2 relPos = b->position - a->position;
	float len = relPos.length();
	float depth = len - c1->radius - c2->radius;
	if (depth < 0){
		cd->penetration = len - c1->radius - c2->radius;
		cd->contactNormal = relPos;
		cd->contactNormal.normalize();
		cd->contactPoint[0].x = cd->contactPoint[0].y = 0;
		return true;
	}
	else
		return false;
}


void collider::addExtent(extent& e, const vec2& axis, const vec2& v, const unsigned char c){
	const float dot = axis.dot(v);
	if (floatLessThanBias(dot, e.min)){
		if (floatEqualToBias(dot, e.min))
			e.min_point |= c;
		else
			e.min_point = c;
		e.min = dot;
	}
	if (floatLessThanBias(e.max, dot)){
		if (floatEqualToBias(dot, e.max))
			e.max_point |= c;
		else
			e.max_point = c;
		e.max = dot;
	}
}

bool collider::collideBoxBox(body* a, body* b, contactdetails* cd){
	box* c1 = (box*)(a->form), *c2 = (box*)(b->form);
	vec2 relPos = b->position - a->position;
	vec2 b1 = c2->a + relPos, b2 = c2->b + relPos, b3 = c2->c + relPos, b4 = c2->d + relPos;
	float orientation = b->rotation - a->rotation;
	float cs = cos(-a->rotation), sn = sin(-a->rotation);
	float cs2 = cos(orientation), sn2 = sin(orientation);
	b1.rotate(cs, sn);
	b2.rotate(cs, sn);
	b3.rotate(cs, sn);
	b4.rotate(cs, sn);
	relPos.rotate(cs, sn);
	vec2 axis1 = vec2(cs2, sn2), axis2 = vec2(-sn2, cs2),
		axisx = vec2(1, 0), axisy = vec2(0, 1);
	const vec2 a1 = vec2(-c1->halfwidth, c1->halfheight),
		a2 = vec2(c1->halfwidth, c1->halfheight),
		a3 = vec2(-c1->halfwidth, -c1->halfheight),
		a4 = vec2(c1->halfwidth, -c1->halfheight);
	extent a_axisx = { -c1->halfwidth, c1->halfwidth, 5, 10 },
		a_axisy = { -c1->halfheight, c1->halfheight, 3, 12 },
		a_axis1 = { axis1.dot(a1), axis1.dot(a1),1,1 },
		a_axis2 = { axis2.dot(a1), axis2.dot(a1),1,1 },
		b_axis1 = { relPos.dot(axis1)-c2->halfwidth, relPos.dot(axis1)+c2->halfwidth, 5, 10 },
		b_axis2 = { relPos.dot(axis2)-c2->halfheight, relPos.dot(axis2)+c2->halfheight, 3, 12 },
		b_axisx = { axisx.dot(b1), axisx.dot(b1), 1,1 },
		b_axisy = { axisy.dot(b1), axisy.dot(b1), 1,1 };
	addExtent(a_axis1, axis1, a2,2); addExtent(a_axis1, axis1, a3,4); addExtent(a_axis1, axis1, a4,8);
	addExtent(a_axis2, axis2, a2,2); addExtent(a_axis2, axis2, a3,4); addExtent(a_axis2, axis2, a4,8);
	addExtent(b_axisx, axisx, b2,2); addExtent(b_axisx, axisx, b3,4); addExtent(b_axisx, axisx, b4,8);
	addExtent(b_axisy, axisy, b2,2); addExtent(b_axisy, axisy, b3,4); addExtent(b_axisy, axisy, b4,8);
	if (!overlap(a_axisx, b_axisx) || !overlap(a_axisy,b_axisy) ||
		!overlap(a_axis1,b_axis1) || !overlap(a_axis2,b_axis2))
		return false;
	float penX = penetration(a_axisx, b_axisx),
		penY = penetration(a_axisy,b_axisy),
		pen1 = penetration(a_axis1,b_axis1),
		pen2 = penetration(a_axis2,b_axis2);
	if (penX < penY && penX < pen1 && penX < pen2){
		cd->penetration = penX;
		axisx.rotate(-cs, sn);
		if (a_axisx.min > b_axisx.min)
			cd->contactNormal = axisx;
		else
			cd->contactNormal = -1 * axisx;
	}
	else if (penY < pen1 && penY < pen2){
		cd->penetration = penY;
		axisy.rotate(-cs, sn);
		if (a_axisy.min > b_axisy.min)
			cd->contactNormal = axisy;
		else
			cd->contactNormal = -1 * axisy;
	}
	else if (pen1 < pen2){
		cd->penetration = pen1;
		axis1.rotate(-cs, sn);
		if (b_axis1.min < a_axis1.min)
			cd->contactNormal = axis1;
		else
			cd->contactNormal = -1 * axis1;
	}
	else{
		cd->penetration = pen2;
		axis2.rotate(-cs, sn);
		if (b_axis2.min < a_axis2.min)
			cd->contactNormal = axis2;
		else
			cd->contactNormal = -1 * axis2;
	}
	return true;
}


bool collider::detectCollision(body* a, body* b, contactdetails* cd){
	if (a->form->type == CIRCLE_TYPE && b->form->type == CIRCLE_TYPE){
		return collideCircleCircle(a, b, cd);
	}
	if (a->form->type == BOX_TYPE && b->form->type == BOX_TYPE){
		return collideBoxBox(a, b, cd);
	}
	return false;
}

