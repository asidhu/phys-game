#include "PhysGame\Source\GameInputHandler.h"
#include "MyEngine.h"

class MyInputHandler : public GameInputHandler{
	MyEngine* engine;
public:
	void handleKey(int key, int state);
	void handleMouse(float x, float y, int button, int state){}
	void handleMouseMove(float x, float y){}
};