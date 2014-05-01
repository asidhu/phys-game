#include "MyEngine.h"
#include "PhysGame\Source\GameWorld.h"
#include "ProjectilePathRenderer.h"
#include "PhysicsEngine\Source\PhysEngine.h"
#include <cassert>
void MyEngine::slowTime(float t){
	game_engine->setTimeStep(t);
}

void MyEngine::enableProjectilePath(GameObject* o){
}
void MyEngine::setProjectilePathVisibility(bool b){
	m_renderPaths = b;
	m_player->mRenderProjectilePath = b;
}
MyEngine::MyEngine(){
	m_player = 0L;
	m_renderPaths = false;
	path_rendering_specifications.ai = .8f;
	path_rendering_specifications.af = 0.f;
	path_rendering_specifications.alive = &m_renderPaths;
	path_rendering_specifications.r = 1.f;
	path_rendering_specifications.g = path_rendering_specifications.b = 0.f;
	path_rendering_specifications.ti = 0;
	path_rendering_specifications.tf = 2.f;
	path_rendering_specifications.half_gravity=-9.81f/2;
	path_rendering_specifications.toff = 0;
	path_rendering_specifications.num = 10;

}

void MyEngine::slomo(bool b){
	slow_time = b;
	setProjectilePathVisibility(b);
	if (b){
		slowTime(1 / 360.f);
	}
	else{
		slowTime(1 / 60.f);
	}
}