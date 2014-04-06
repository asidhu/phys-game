
#include "math.h"
class body;
class contactdetails;
namespace collider{
	struct extent{
		float min, max;
		unsigned char min_point, max_point;
	};
	/*
	void findExtent(vec2& output,const vec2& a,const vec2& b,const vec2& c,const vec2& d, unsigned char pt){
		output = vec2(0, 0);
		int count = 0;
		if ((pt & 1) != 0){
			output += a;
			count++;
		}
		if ((pt & 2) != 0){
			output += b;
			count++;
		}
		if ((pt & 4) != 0){
			output += c;
			count++;
		}
		if ((pt & 8) != 0){
			output += d;
			count++;
		}
		output *= 1.f / count;
	}
	*/
	bool collideBoxBox(body*, body*, contactdetails*);
	bool collideCircleCircle(body*, body*, contactdetails*);
	bool detectCollision(body*, body*, contactdetails* );

	
	void addExtent(extent& e, const vec2& axis, const vec2& v, const unsigned char);
}