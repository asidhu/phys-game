#include "MyEngine.h"
#include "PhysGame\Source\GameWorld.h"
#include "ProjectilePathRenderer.h"
#include "PhysicsEngine\Source\PhysEngine.h"
#include <cassert>
void MyEngine::slowTime(float t){
	game_engine->setTimeStep(t);
}

void MyEngine::enableProjectilePath(GameObject* o){
	assert(projectilePathsVisible);
	ProjectilePathRenderer* pathRenderer = new ProjectilePathRenderer(o);
	pathRenderer->ti = 0;
	pathRenderer->tf = 2.f;
	pathRenderer->ai = .8f;
	pathRenderer->af = 0;
	pathRenderer->r = 1;
	pathRenderer->b = pathRenderer->g = 0;
	pathRenderer->num = 10;
	pathRenderer->half_gravity = game_engine->getGameWorld()->m_physEngine->getGravity();
	pathRenderer->alive = &projectilePathsVisible;
	//m_projectilepaths.push_back(pathRenderer);
	game_engine->getGameWorld()->addEffect(pathRenderer);
}
void MyEngine::setProjectilePathVisibility(bool b){
	if (b && !projectilePathsVisible){
		projectilePathsVisible = true;
		std::list<Actor*>::iterator it = game_engine->getGameWorld()->m_actors.begin(),
			end = game_engine->getGameWorld()->m_actors.end();
		for (; it != end;it++)
		{
			GameObject* o = (GameObject*)(*it);
			if (o->projectilePathVisible){
				enableProjectilePath(o);
			}
		}
	}
	if (!b && projectilePathsVisible){
		projectilePathsVisible = false;
	}
}