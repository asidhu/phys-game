#include "EnemyDriller.h"
#include "PhysicsEngine\Source\PhysEngine.h"
#include "PhysGame\Source\GameEngine.h"
#include "Missile.h"
#include "Player.h"

#define max(a,b) ((a<b)?b:a)


EnemyDriller::EnemyDriller(body* b) :Mob(0, b){
	fire = 0;
	jump = 0;
}
bool EnemyDriller::tick(GameEngine* e){
	if ((fire++ % 50) == 0){
		Player* p = (Player*)e->player;
		float dx = p->getBody()->position.x - getBody()->position.x;
		float nxtY = getBody()->position.y + (float)rand() / RAND_MAX*abs(dx)*2.f;
	}
	/*if (((jump+=rand()%3) % 300) == 0){
	jump = 0;
	getBody()->applyImpulse(vec2(rand()%10-5, 20));
	}*/
	return Mob::tick(e);
}
void EnemyDriller::render(RenderList*){

}