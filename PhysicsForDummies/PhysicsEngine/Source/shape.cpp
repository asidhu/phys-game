#include "shape.h"
#include "rect.h"
#include "Body.h"
#include "collision.h"
#define max(a,b) ((a>b)?a:b)
#define min(a,b) ((a<b)?a:b)
inline bool overlap(const extent& a,const extent& b){
	return !(a.max<b.min || a.min>b.max);
}
inline bool overlap(const extent& a, const float min, const float max){
	return !(a.max<min || a.min>max);
}
inline float halfextent(const extent& a){
	return (a.max - a.min) / 2;
}
inline vec2 extent2point(const unsigned char a,const box* b){
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
inline bool floatEqualToBias(const float a, const float b){
	const float bias = .0001f;
	return fabs(a - b) < bias;
}
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
	if (floatEqualToBias(axis1_span.min, bd1))
		axis1_span.min_point |= 2;
	if (floatEqualToBias(axis1_span.min, cd1))
		axis1_span.min_point |= 4;
	if (floatEqualToBias(axis1_span.min, dd1))
		axis1_span.min_point |= 8;

	if (floatEqualToBias(axis1_span.max, ad1))
		axis1_span.max_point |= 1;
	if (floatEqualToBias(axis1_span.max, bd1))
		axis1_span.max_point |= 2;
	if (floatEqualToBias(axis1_span.max, cd1))
		axis1_span.max_point |= 4;
	if (floatEqualToBias(axis1_span.max, dd1))
		axis1_span.max_point |= 8;;

	if (floatEqualToBias(axis2_span.min, ad2))
		axis2_span.min_point |= 1;
	if (floatEqualToBias(axis2_span.min, bd2))
		axis2_span.min_point |= 2;
	if (floatEqualToBias(axis2_span.min, cd2))
		axis2_span.min_point |= 4;
	if (floatEqualToBias(axis2_span.min, dd2))
		axis2_span.min_point |= 8;

	if (floatEqualToBias(axis2_span.max, ad2))
		axis2_span.max_point |= 1;
	if (floatEqualToBias(axis2_span.max, bd2))
		axis2_span.max_point |= 2;
	if (floatEqualToBias(axis2_span.max, cd2))
		axis2_span.max_point |= 4;
	if (floatEqualToBias(axis2_span.max, dd2))
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

bool shape::detectCollision(body* a, body* b, contactdetails* cd){
	if (a->form->type == CIRCLE_TYPE && b->form->type == CIRCLE_TYPE){
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
	if (a->form->type == BOX_TYPE && b->form->type == BOX_TYPE){
		box* c1 = (box*)(a->form), *c2 = (box*)(b->form);
		vec2 relPos = b->position - a->position;
		vec2 ax1, ax2, ax3, ax4;
		vec2 orientedRelPos1, orientedRelPos2;
		ax1.x = cos(a->rotation);
		ax1.y = sin(a->rotation);
		ax2.x = -ax1.y;
		ax2.y = ax1.x;
		extent axis1_extent, axis2_extent,axis3_extent, axis4_extent;
		c2->getProjections(relPos,ax1,ax2, axis1_extent, axis2_extent);
		if (!(overlap(axis1_extent,-c1->halfwidth,c1->halfwidth) && overlap(axis2_extent,-c1->halfheight,c1->halfheight)))
			return false;
		orientedRelPos1.x = -ax1.dot(relPos);
		orientedRelPos1.y = -ax2.dot(relPos);
		float penX = fabs(orientedRelPos1.x) - c1->halfwidth - halfextent(axis1_extent),
			penY = fabs(orientedRelPos1.y) - c1->halfheight - halfextent(axis2_extent);
		float pen1 = 0,
			pen2 = 0;
		if (b->rotation!=a->rotation){
			ax3.x = cos(b->rotation);
			ax3.y = sin(b->rotation);
			ax4.x = -ax3.y;
			ax4.y = ax3.x;
			orientedRelPos2.x = -ax3.dot(relPos);
			orientedRelPos2.y = -ax4.dot(relPos);
			vec2 negrelPos = -1 * relPos;
			c1->getProjections(negrelPos, ax3, ax4, axis3_extent, axis4_extent);
			if (!(overlap(axis3_extent,-c2->halfwidth,c2->halfwidth) && overlap(axis4_extent,-c2->halfheight,c2->halfheight)))
				return false;
			pen1 = fabs(orientedRelPos2.x) - c2->halfwidth - halfextent(axis3_extent);
			pen2 = fabs(orientedRelPos2.y) - c2->halfheight - halfextent(axis4_extent);
		}
		else pen1 = pen2 = min(penX, penY) - 1;

		
			if (penY > penX){
				cd->penetration = penY;
				if (orientedRelPos1.y < 0){
					cd->contactNormal =  ax2;
					cd->contactPoint[1] = extent2point(axis2_extent.min_point, c2);
					//cd->contactPoint[0] = cd->contactPoint[1] + relPos;
				}
				else{
					cd->contactNormal = -1 * ax2;
					cd->contactPoint[1] = extent2point(axis2_extent.max_point, c2);
					//cd->contactPoint[0] = cd->contactPoint[1] + relPos;
				}
			}
			else{
				cd->penetration = penX;
				if (orientedRelPos1.x < 0){
					cd->contactNormal = ax1;
					cd->contactPoint[1] = extent2point(axis1_extent.min_point, c2);
					//cd->contactPoint[0] = cd->contactPoint[1] + relPos;
				}  
				else{
					cd->contactNormal = -1 * ax1;
					cd->contactPoint[1] = extent2point(axis1_extent.max_point, c2);
					//cd->contactPoint[0] = cd->contactPoint[1] + relPos;
				}
			}
			if (a->rotation != b->rotation)
			{
				if (pen1 > pen2){
					if (orientedRelPos2.x < 0){
						if (pen1>cd->penetration)
							cd->contactNormal = ax3;
						cd->contactPoint[0] = extent2point(axis3_extent.max_point, c1);
						//cd->contactPoint[1] = cd->contactPoint[0] - relPos;
					}
					else{
						if (pen1 > cd->penetration)
							cd->contactNormal = -1 * ax3;
						cd->contactPoint[0] = extent2point(axis3_extent.min_point, c1);
						//cd->contactPoint[1] = cd->contactPoint[0] - relPos;
					}

					if (pen1 > cd->penetration){
						cd->penetration = pen1;
					}
				}
				else{
					if (orientedRelPos2.y < 0){
						if (pen1>cd->penetration)
							cd->contactNormal = ax4;
						cd->contactPoint[0] = extent2point(axis4_extent.max_point, c1);
						//cd->contactPoint[1] = cd->contactPoint[0] - relPos;
					}
					else{
						if (pen1 > cd->penetration)
							cd->contactNormal = -1 * ax4;
						cd->contactPoint[0] = extent2point(axis4_extent.min_point, c1);
						//cd->contactPoint[1] = cd->contactPoint[0] - relPos;
					}

					if (pen2 > cd->penetration){
						cd->penetration = pen2;
					}
				}
			}
			if (fabs(cd->penetration) > .15f){
				cd->penetration *= 1.f;
			}
			return true;
	}
	return false;
}