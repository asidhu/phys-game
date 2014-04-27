#include "PhysGame\Source\GameInputHandler.h"
#include "MyEngine.h"

class MyInputHandler : public GameInputHandler{
	MyEngine* engine;
	bool A_DOWN, S_DOWN, W_DOWN, D_DOWN, RIGHT_DOWN;
public:
	MyInputHandler(MyEngine* e){ engine = e; A_DOWN = S_DOWN = W_DOWN = D_DOWN= RIGHT_DOWN = false; }
	void handleKey(int key, int state);
	void handleMouse(float x, float y, int button, int state);
	void handleMouseMove(float x, float y){}
	void tick(float timestep);
};