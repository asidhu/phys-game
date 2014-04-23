#pragma once

class Camera{
public:
	float l, r, t, b;
	float w(){
		return r - l;
	}
	float h(){
		return b - t;
	}
};