#include "Player.h"
#include "Grapple.h"
#include "Missile.h"
#include "PhysGame\Source\GameEngine.h"
#include "PhysicsEngine\Source\BodyDef.h"
#include "PhysicsEngine\Source\PhysEngine.h"
#include "PhysGame\Source\RenderList.h"
#define max(a,b) ((a<b)?b:a)
#define PROJECTILESPEED 45
#define INVULNERABLETIME 300.f
void onGroundCheck(body* b, contactdetails* dets){
	const vec2 up = vec2(0, 1);
	if (((Player*)b->data)->onGround>100 || dets->contactNormal.dot(up) < .5f)
		return;
	float relV = b->velocity.dot(up);
	if (fabs(relV) < 1)
		((Player*)b->data)->onGround++;
}
Player::Player(int id, body* b) :Mob(id, b){
	grappleHook = NULL;
	b->post_collide = onGroundCheck;
	dmgfx = 0;
}
bool Player::tick(GameEngine* e){
	if(dmgfx>0)dmgfx--;
	if (m_hp<=0 && m_deathframes++>500)
	{
		e->close();
		return false;
	}
	return false;
}


void Player::dmg(int d){
	if (dmgfx <= 0){
		m_hp -= d;
		dmgfx = INVULNERABLETIME;
	}
}

body* createBody2(PhysEngine* engine, float x, float y, float w, float h, float mass, float rot = 0){
	bodydef bdef;
	bdef.position.x = x;
	bdef.position.y = y;
	bdef.width = w;
	bdef.height = h;
	bdef.mass = mass;
	bdef.rotation = rot;
	return engine->buildBody(bdef);
}
void Player::fireMissile(GameEngine* e,float x, float y){
	vec2 dist = (vec2(x, y) - getBody()->position);
	dist.normalize();
	body* b = createBody2(e->getPhysEngine(), getBody()->position.x, getBody()->position.y, 1.f, 1.f, 1.f, atan2(dist.y, dist.x) * 180 / 3.14159f);
	Missile *a = new Missile(0, b);
	dist *= PROJECTILESPEED + max(dist.dot(getBody()->velocity), 0);
	b->velocity += dist;
	e->addActor(a);
}

void Player::render(RenderList* lst){
	Mob::render(lst);
	body* b = getBody();
	/*if (dmgfx > 0){
		RenderItem* item = lst->getItem();
		item->x = b->position.x;
		item->y = b->position.y;
		item->myType = solidsquare;
		item->rot = b->rotation;
		item->square.lw = 1;
		item->square.w = ((box*)b->form)->halfwidth * 2;
		item->square.h = ((box*)b->form)->halfheight * 2;
		item->square.a = INVULNERABLETIME/dmgfx;
		item->square.r = 1;// ((dmgfx % 8) > 4) ? 1 : 0;
		item->square.g = item->square.b = 0;
		lst->addItem(item);
	}*/
}

body* createBody3(PhysEngine* engine, float x, float y, float w, float h, float mass, float rot = 0){
	bodydef bdef;
	bdef.position.x = x;
	bdef.position.y = y;
	bdef.width = w;
	bdef.height = h;
	bdef.mass = mass;
	bdef.rotation = rot;
	return engine->buildBody(bdef);
}

void Player::fireGrappleHook(float x, float y){
	if (grappleHook == NULL){
		vec2 dist = (vec2(x, y) - getBody()->position);
		dist.normalize();
		body* b = createBody3(engine->getPhysEngine(), getBody()->position.x, getBody()->position.y, 1.3f,1.3f,.1f,0);
		grappleHook = new Grapple(0, b);
		grappleHook->player = this;
		grappleHook->force = 2.f;
		grappleHook->speed = PROJECTILESPEED-1;
		dist *= grappleHook->speed + max(dist.dot(getBody()->velocity),0);
		b->velocity += dist;
		b->data = grappleHook;
		engine->addActor(grappleHook);
	}
}

void Player::fireSecondHook(float x, float y){
	if (grappleHook == NULL || grappleHook->otherHook!=NULL)
		return;
	vec2 dist = (vec2(x, y) - getBody()->position);
	dist.normalize();
	body* b = createBody3(engine->getPhysEngine(), getBody()->position.x, getBody()->position.y, 1.3f, 1.3f, .1f, 0);
	grappleHook->otherHook = b;
	dist *= grappleHook->speed + max(dist.dot(getBody()->velocity), 0);
	b->velocity += dist;
	grappleHook->createOtherHook(b);
	grappleHook = NULL;
}

void Player::releaseGrappleHook(){
	if (grappleHook != NULL){
		grappleHook->shouldreturn = true;
	}
}