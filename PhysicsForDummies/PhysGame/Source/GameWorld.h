
#include <list>
#include "mem\MemAccessPool.h"
class Effect;
class Actor;
class PhysEngine;
class Camera;
class RenderList;
class GameEngine;
class MemAccessPool;
class GameWorld{
private:
	MemAccessPool* m_effects_mempool,
	*m_actors_mempool;
public:
	GameEngine* m_engine;
	GameWorld();
	void tick(float timestep);
	void render(RenderList*, Camera* viewport, Camera* window);
	void addActor(Actor* a);
	void addEffect(Effect* e);
	template<class T, typename... ConstructorArgs>
		T* allocateActor(ConstructorArgs... args){
			return m_actors_mempool->allocate<T>(args...);
		}
	template<class T, typename... ConstructorArgs>
	T* allocateEffect(ConstructorArgs... args){
		return m_effects_mempool->allocate<T>(args...);
	}
	void* allocateEffect(int size);
	void* allocateActor(int size);
	PhysEngine* m_physEngine;
};