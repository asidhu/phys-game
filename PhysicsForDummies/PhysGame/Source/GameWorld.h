
#include <list>

class Effect;
class Actor;
class PhysEngine;
class Camera;
class RenderList;
class GameWorld{
public:
	GameWorld();
	void tick(float timestep);
	void render(RenderList*, Camera* viewport, Camera* window);
	PhysEngine* m_physEngine;
	std::list<Effect*> m_effects;
	std::list<Actor*> m_actors;
};