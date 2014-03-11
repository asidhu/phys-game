
#include "math.h"
class body;
class contactdetails;
namespace collider{
	
	bool collideBoxBox(body*, body*, contactdetails*);
	bool collideCircleCircle(body*, body*, contactdetails*);
	bool detectCollision(body*, body*, contactdetails* );

	struct extent{
		float min, max;
		unsigned char min_point, max_point;
	};
	void addExtent(extent& e, const vec2& axis, const vec2& v, const unsigned char);
}