#include "math.h"


vec2& vec2::operator+=(const vec2& b){
	x = x + b.x;
	y = y + b.y;
	return *this;
}
vec2& vec2::operator-=(const vec2& b){
	x = x - b.x;
	y = y - b.y;
	return *this;
}
vec2& vec2::operator*=(const float b){
	x = b*x;
	y = b*y;
	return *this;
}

void vec2::normalize(){
	float d = length();
	x = x / d;
	y = y / d;
}