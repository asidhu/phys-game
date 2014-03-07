#pragma once
#include "PhysGame\Source\GameEngine.h"
#define KEY_DOWN		1
#define KEY_UP			2
#define MOUSE_PRESS		3
#define MOUSE_RELEASE	4
#define MOUSE_LEFT		5
#define MOUSE_RIGHT		6

class GameEngine;
class PlatformGraphics;
class PlatformInput{
public:
	GameEngine* m_engine; ///this is a hack. 
	GameInputHandler* getInputHandler(){ return m_engine->m_inputHandler; }
	PlatformGraphics* getGraphics(){ return m_engine->m_graphics; }
	virtual void pollInput()=0;
};
