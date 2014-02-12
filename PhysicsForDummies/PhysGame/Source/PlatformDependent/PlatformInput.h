#pragma once

#define KEY_DOWN		1
#define KEY_UP			2
#define MOUSE_PRESS		3
#define MOUSE_RELEASE	4
#define MOUSE_LEFT		5
#define MOUSE_RIGHT		6

class PlatformInput{
public:
	void (*onKey)(int key, int state);
	void (*onMouse)(float x, float y, int mouse, int state);
	void (*onMouseMove)(float x, float y);
	virtual void pollInput();
};
