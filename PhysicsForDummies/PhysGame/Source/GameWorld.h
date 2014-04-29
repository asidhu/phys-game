
#include <list>

class Effect;
class Actor;
class PhysEngine;
class Camera;
class RenderList;
class GameEngine;
class GameWorld{
	
public:
	GameEngine* m_engine;
	GameWorld();
	void tick(float timestep);
	void render(RenderList*, Camera* viewport, Camera* window);
	void addActor(Actor* a);
	void addEffect(Effect* e){
		m_effects.push_back(e);
	}
	PhysEngine* m_physEngine;
	std::list<Effect*> m_effects;
	std::list<Actor*> m_actors;
};