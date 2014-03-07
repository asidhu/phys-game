#include "shape.h"
#include "rect.h"
#include "Body.h"
#include "collision.h"
#define max(a,b) ((a>b)?a:b)
#define min(a,b) ((a<b)?a:b)

void circle::calcAABB(body* b, rect* aabb){
	b->boundingArea.left = -radius;
	b->boundingArea.right = radius;
	b->boundingArea.top = radius;
	b->boundingArea.bottom = -radius;
	aabb->left = -radius + b->position.x;
	aabb->right = radius + b->position.x;
	aabb->top = radius + b->position.y;
	aabb->bottom = -radius + b->position.y;
}
void box::calcAABB(body* b, rect* aabb){
	calcOBB(b->rotation);
	float minX, maxX, minY, maxY;
	vec2 zero;
	getProjectionsXY(zero, minX, maxX, minY, maxY);
	b->boundingArea.left = minX;
	b->boundingArea.right = maxX;
	b->boundingArea.top = maxY;
	b->boundingArea.bottom = minY;
	aabb->left = minX+b->position.x;
	aabb->right = maxX + b->position.x;
	aabb->top = maxY + b->position.y;
	aabb->bottom = minY + b->position.y;
}
void box::calcOBB(float rot){
	float cs = cos(rot), sn = sin(rot);
	d.x = halfwidth*cs + halfheight*sn;
	a.x = -d.x;//-(halfwidth*cs + halfheight*sn);
	a.y = -halfwidth*sn + halfheight*cs;
	b.x =  halfwidth*cs - halfheight*sn;
	b.y =  halfwidth*sn + halfheight*cs;
	c.x = -b.x;// -halfwidth*cs + halfheight*sn;
	c.y = -(b.y);//halfwidth*sn + halfheight*cs);
	//d.x =  halfwidth*cs + halfheight*sn; //move up
	d.y = -a.y;//halfwidth*sn - halfheight*cs;
}

inline bool floatEqualToBias(const float a, const float b){
	const float bias = .00000001f;
	return fabs(a - b) < bias;
}


void box::getProjections(vec2& relPos, vec2& axis1, vec2& axis2, extent& axis1_span, extent& axis2_span){
	vec2 a = relPos + this->a, b = relPos + this->b,
		c = relPos + this->c, d = relPos + this->d;
	float ad1 = axis1.dot(a), ad2 = axis2.dot(a),
		bd1 = axis1.dot(b), bd2 = axis2.dot(b),
		cd1 = axis1.dot(c), cd2 = axis2.dot(c),
		dd1 = axis1.dot(d), dd2 = axis2.dot(d);
	axis1_span.min_point = axis1_span.max_point = axis2_span.min_point = axis2_span.max_point = 0;
	axis1_span.min = min(min(ad1, bd1), min(cd1, dd1));
	axis1_span.max = max(max(ad1, bd1), max(cd1, dd1));
	axis2_span.min = min(min(ad2, bd2), min(cd2, dd2));
	axis2_span.max = max(max(ad2, bd2), max(cd2, dd2));
	if (floatEqualToBias(axis1_span.min,ad1))
		axis1_span.min_point |= 1;
	else if (floatEqualToBias(axis1_span.min, bd1))
		axis1_span.min_point |= 2;
	else if (floatEqualToBias(axis1_span.min, cd1))
		axis1_span.min_point |= 4;
	else if (floatEqualToBias(axis1_span.min, dd1))
		axis1_span.min_point |= 8;

	if (floatEqualToBias(axis1_span.max, ad1))
		axis1_span.max_point |= 1;
	else if (floatEqualToBias(axis1_span.max, bd1))
		axis1_span.max_point |= 2;
	else if (floatEqualToBias(axis1_span.max, cd1))
		axis1_span.max_point |= 4;
	else if (floatEqualToBias(axis1_span.max, dd1))
		axis1_span.max_point |= 8;;

	if (floatEqualToBias(axis2_span.min, ad2))
		axis2_span.min_point |= 1;
	else if (floatEqualToBias(axis2_span.min, bd2))
		axis2_span.min_point |= 2;
	else if (floatEqualToBias(axis2_span.min, cd2))
		axis2_span.min_point |= 4;
	else if (floatEqualToBias(axis2_span.min, dd2))
		axis2_span.min_point |= 8;

	if (floatEqualToBias(axis2_span.max, ad2))
		axis2_span.max_point |= 1;
	else if (floatEqualToBias(axis2_span.max, bd2))
		axis2_span.max_point |= 2;
	else if (floatEqualToBias(axis2_span.max, cd2))
		axis2_span.max_point |= 4;
	else if (floatEqualToBias(axis2_span.max, dd2))
		axis2_span.max_point |= 8;

}
void box::getProjectionsXY(vec2& relPos, float& minX, float& maxX, float& minY, float& maxY){
	vec2 a = relPos + this->a, b = relPos + this->b,
		c = relPos + this->c, d = relPos + this->d;
	minX = min(min(a.x, b.x), min(c.x, d.x));
	maxX = max(max(a.x, b.x), max(c.x, d.x));
	minY = min(min(a.y, b.y), min(c.y, d.y));
	maxY = max(max(a.y, b.y), max(c.y, d.y));
}