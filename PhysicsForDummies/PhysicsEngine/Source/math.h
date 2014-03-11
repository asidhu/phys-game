#pragma once
#include <cmath>
class vec2{
public:
	inline vec2(){
		x = y = 0;
	}
	vec2(float x, float y){
		this->x = x; this->y = y;
	}
	vec2& operator+=(const vec2&);
	vec2& operator-=(const vec2&);
	vec2& operator*=(const float);
	bool operator==(const vec2& o) const{
		return x == o.x && y == o.y;
	}
	bool operator!=(const vec2& o) const{
		return !operator==(o);
	}
	inline float lengthSq()const{ return x*x + y*y; }
	inline float length()const { return sqrtf(x*x + y*y); }
	inline float dot(const vec2& b)const { return x*b.x + y*b.y; }
	inline float crossZ(const vec2& b)const { return x*b.y - y*b.x; }
	inline void rotate(const float cs, const float sn){
		const float x = this->x, y = this->y;
		this->x = x*cs - y*sn;
		this->y = x*sn + y*cs;
	}
	void normalize();
	float x, y;
};

class AABB{
	float width;
	float height;
};

inline vec2 operator+(vec2 lhs, const vec2& rhs)
{
	lhs += rhs;
	return lhs;
}
inline vec2 operator-(vec2 lhs, const vec2& rhs)
{
	lhs -= rhs;
	return lhs;
}
inline vec2 operator*(vec2 lhs, const float rhs)
{
	lhs *= rhs;
	return lhs;
}
inline vec2 operator*(const float rhs, const vec2 lhs){ return lhs*rhs; }