#pragma once

class GameInputHandler{
public:
	virtual void handleKey(int key, int state) = 0;
	virtual void handleMouse(float x, float y, int button, int state) = 0;
	virtual void handleMouseMove(float x, float y) = 0;
};