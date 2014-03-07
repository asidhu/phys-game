#include "EnemyCannoneer.h"
#include "PhysicsEngine\Source\PhysEngine.h"
#include "PhysGame\Source\GameEngine.h"
#include "Missile.h"
#include "Player.h"

#include <cstdlib>
#define max(a,b) ((a<b)?b:a)
body* createBody64(PhysEngine* engine, float x, float y, float w, float h, float mass, float rot = 0){
	bodydef bdef;
	bdef.position.x = x;
	bdef.position.y = y;
	bdef.width = w;
	bdef.height = h;
	bdef.mass = mass;
	bdef.rotation = rot;
	return engine->buildBody(bdef);
}

EnemyCannoneer::EnemyCannoneer(body* b):Mob(0,b){
	fire = 0;
	jump = 0;
}
bool EnemyCannoneer::tick(GameEngine* e){
	if ((fire++ % 50)==0){
		Player* p = (Player*)e->player;
		float dx = p->getBody()->position.x - getBody()->position.x;
		if (fabs(dx) > 30){
			float nxtX = getBody()->position.x + (float)rand() / RAND_MAX*dx*.4f;
			fireMissile(e, nxtX, 90);
		}
	}
	/*if (((jump+=rand()%3) % 300) == 0){
		jump = 0;
		getBody()->applyImpulse(vec2(rand()%10-5, 20));
	}*/
	return Mob::tick(e);
}
void EnemyCannoneer::fireMissile(GameEngine* e, float x, float y){
	vec2 dist = (vec2(x, y) - getBody()->position);
	dist.normalize();
	body* b = createBody64(e->getPhysEngine(), getBody()->position.x, getBody()->position.y, 1.f, 1.f, 1.f, atan2(dist.y, dist.x) * 180 / 3.14159f);
	Missile *a = new Missile(0, b);
	dist *= 40 + max(dist.dot(getBody()->velocity), 0);
	b->velocity += dist;
	e->addActor(a);
	a->launcher = this;
}