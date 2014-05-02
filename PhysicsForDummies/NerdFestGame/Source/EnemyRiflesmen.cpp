#include "EnemyRiflesmen.h"
#include "PhysicsEngine\Source\PhysEngine.h"
#include "PhysGame\Source\GameEngine.h"
#include "Missile.h"
#include "PhysGame\Source\GameWorld.h"
#include "Player.h"
#include <cstdlib>
#include "MyActorManager.h"
#define max(a,b) ((a<b)?b:a)


EnemyRiflesmen::EnemyRiflesmen(body* b) :Mob(0, b){
	fire = 0;
	target = NULL;
	b->dataFlag |= ISENEMY;
	m_hp = 100;
}
bool EnemyRiflesmen::tick(float timestep, GameWorld* e){
	if (hasTarget()){
		GameObject* o = target->getdata();
		vec2 dist = o->getBody()->position - getBody()->position;
		if (dist.length() > 100){
			releaseTarget();
		}
		else if(((fire += timestep))>.5f){
			float dx = o->getBody()->position.x - getBody()->position.x;
			float nxtY = getBody()->position.y + (float)rand() / RAND_MAX*abs(dx)*2.f;
			fireMissile(e, o->getBody()->position.x, nxtY);
			fire = 0;
		}
	}
	/*if (((jump+=rand()%3) % 300) == 0){
	jump = 0;
	getBody()->applyImpulse(vec2(rand()%10-5, 20));
	}*/
	if (m_hp < 0)
		return false;
	return Mob::tick(timestep, e);
}
void EnemyRiflesmen::fireMissile(GameWorld* e, float x, float y){
	
	vec2 dist = (vec2(x, y) - getBody()->position);
	dist.normalize();
	body* b = am_createbody(e->m_physEngine, getBody()->position.x, getBody()->position.y, 1.f, 1.f, 1.f, atan2(dist.y, dist.x) * 180 / 3.14159f);
	Missile *a = e->allocateActor<Missile>(0, b);
	dist *= 40 + max(dist.dot(getBody()->velocity), 0);
	b->velocity += dist;
	e->addActor(a);
	a->launcher = this;
}