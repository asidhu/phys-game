#include "PhysGame\Source\GameInputHandler.h"
#include "MyEngine.h"
#include <map>
class MyInputHandler : public GameInputHandler{
	MyEngine* engine;
	std::map<int, int> m_keys;
	float chargeTime;
	float mX, mY;
	int RIGHT_DOWN, LEFT_DOWN;
public:
	MyInputHandler(MyEngine* e){ engine = e; LEFT_DOWN = RIGHT_DOWN = 0; }
	void handleKey(int key, int state);
	void handleMouse(float x, float y, int button, int state);
	void handleMouseMove(float x, float y);
	void tick(float timestep);
	bool isDown(int key);
	bool isUp(int key);
	bool justReleased(int key);
};