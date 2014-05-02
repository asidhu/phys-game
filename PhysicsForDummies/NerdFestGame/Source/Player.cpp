#include "Player.h"
#include "Grapple.h"
#include "Missile.h"
#include "PhysGame\Source\GameEngine.h"
#include "PhysicsEngine\Source\BodyDef.h"
#include "PhysicsEngine\Source\PhysEngine.h"
#include "PhysGame\Source\RenderList.h"
#include "MyActorManager.h"
#include "PhysGame\Source\GameWorld.h"
#include "MyEngine.h"
#define max(a,b) ((a<b)?b:a)
#define PROJECTILESPEED 45
#define INVULNERABLETIME 300.f
Player* Player::me = NULL;

void onGroundCheck(body* b, contactdetails* dets){
	const vec2 up = vec2(0, 1);
	Player* p = (Player*)b->data;
	Player::checkExtremeCollision(p, dets);
	if (p->onGround>100 || dets->contactNormal.dot(up) < .5f)
		return;
	float relV = b->velocity.dot(up);
	if (fabs(relV) < 1)
		p->onGround++;

}
Player::Player(int id, body* b) :Mob(id, b){
	grappleHook = NULL;
	b->post_collide = onGroundCheck;
	onGround = 0;
	dmgfx = 0;
	createMissile = fireGrapplingHook = fire2ndGrapplingHook = 0;
	psychicPowerMeter = 0;
	m_hp = 100;
}

bool Player::canSlowTime(){
	return psychicPowerMeter>10;
}
bool Player::canFireMissile(){
	return true;
}
bool Player::canFireSecondHook(){
	return grappleHook != NULL && grappleHook->otherHook == NULL;
}
bool Player::canFireGrapplingHook(){
	return grappleHook == NULL;
}
bool Player::tick(float timestep, GameWorld* e){
	if(dmgfx>0)dmgfx--;
	if (m_hp<=0)
	{
		getBody()->position.x = -50;
		getBody()->position.y = -15;
		getBody()->velocity.x = 0;
		getBody()->velocity.y = 0;
		getBody()->rotation = 0;
		getBody()->angularVelocity = 0;
		m_hp = 100;
		if(grappleHook)
			this->grappleHook->release();
	}

	if (createMissile){
		vec2 dist = (vec2(missileTo.x, missileTo.y) - getBody()->position);
		body* b = am_createbody(e->m_physEngine, getBody()->position.x, getBody()->position.y, 1.f, 1.f, 1.f, atan2(dist.y, dist.x) * 180 / 3.14159f);
		Missile *a = (Missile*)e->allocateActor(sizeof(Missile));
		a = new (a) Missile(0, b);
		dist.normalize();
		dist *= PROJECTILESPEED + max(dist.dot(getBody()->velocity), 0);
		b->velocity += dist;
		e->addActor(a);
		a->launcher = this;
		createMissile = 0;
	}

	if (fireGrapplingHook){
		vec2 dist = (vec2(grapplingHookTo.x, grapplingHookTo.y) - getBody()->position);
		dist.normalize();
		body* b = am_createbody(e->m_physEngine, getBody()->position.x, getBody()->position.y, 1.3f, 1.3f, .1f, 0);
		grappleHook = (Grapple*)e->allocateActor(sizeof(Grapple));
		grappleHook= new (grappleHook)Grapple(0, b);
		grappleHook->player = this;
		grappleHook->force = 2.f;
		grappleHook->speed = PROJECTILESPEED - 1;
		dist *= grappleHook->speed + max(dist.dot(getBody()->velocity), 0);
		b->velocity += dist;
		b->data = grappleHook;
		e->addActor(grappleHook);
		fireGrapplingHook = 0;
	}

	if (fire2ndGrapplingHook){
		vec2 dist = (vec2(grapplingHookTo.x, grapplingHookTo.y) - getBody()->position);
		dist.normalize();
		body* b = am_createbody(e->m_physEngine, getBody()->position.x, getBody()->position.y, 1.3f, 1.3f, .1f, 0);
		grappleHook->otherHook = b;
		dist *= grappleHook->speed + max(dist.dot(getBody()->velocity), 0);
		b->velocity += dist;
		grappleHook->createOtherHook(b);
		grappleHook = NULL;
		fire2ndGrapplingHook = 0;
	}
	if (engine->slow_time){
		if (psychicPowerMeter > 1)
			psychicPowerMeter-=.1f;
		else{
			engine->slomo(false);
		}
	}


	PhysEngine *phys = engine->game_engine->getGameWorld()->m_physEngine;
	me = this;
	phys->findAll(getBody()->position.x, getBody()->position.y, 300, targetme);

	if(psychicPowerMeter<100)
		psychicPowerMeter += 10*timestep;
	return false;
}


void Player::implode(body* b, float r,vec2 diff){
	if (b->dataFlag & OKGRAVITYWELL){
		diff.normalize();
		b->impulse += diff * 10;
	}
}
void Player::targetme(body* b, float r, vec2 diff){
	if (b->dataFlag & ISENEMY){
		Mob* o = (Mob*)b->data;
		o->acquireTarget(me);
	}
}
void Player::explode(body* b, float r, vec2 diff){
	if (b->dataFlag & OKGRAVITYWELL){
		diff.normalize();
		b->impulse -= diff * 10;
	}
}


void Player::initiateGravityWell(float wX, float wY,float radius, bool inwards){
	if (psychicPowerMeter > .5f){
		psychicPowerMeter -= .5f;
		PhysEngine *phys = engine->game_engine->getGameWorld()->m_physEngine;
		setAffectedByGravityWell(false);
		phys->findAll(wX, wY, 20, inwards ? implode : explode);
		setAffectedByGravityWell(true);
	}
}

void Player::renderHealthBar(RenderList* lst){
	const float color1[3] = { 0,1,0 },
		color2[3] = { 1,0,0};
	float t = this->m_hp / 100;
	RenderItem* itm = lst->getItem();
	itm->myType = solidsquare;
	itm->myAnchorX = left;
	itm->myAnchorY = top;
	itm->x = 10;
	itm->y = 65 - 30 * t;
	itm->rot = 0;
	itm->zIndex = -100;
	itm->square.w = 5;
	itm->square.h = 60 * t;
	itm->square.a = 1;
	itm->square.r = color1[0] * t + color2[0] * (1 - t);
	itm->square.g = color1[1] * t + color2[1] * (1 - t);
	itm->square.b = color1[2] * t + color2[2] * (1 - t);
	lst->addItem(itm);
}

void Player::renderPsychicBar(RenderList* lst){
	const float color1[3] = { 153.f/255, 51.f/255, 1 },
		color2[3] = { 51.f/255, 1, 51.f/255 };
	float t = psychicPowerMeter / 100;
	RenderItem* itm = lst->getItem();
	itm->myType = solidsquare;
	itm->myAnchorX = left;
	itm->myAnchorY = top;
	itm->x = 20;
	itm->y = 65-30*t;
	itm->rot = 0;
	itm->zIndex = -100;
	itm->square.w = 5;
	itm->square.h = 60*t;
	itm->square.a = 1;
	itm->square.r = color1[0] * t + color2[0] * (1 - t);
	itm->square.g = color1[1] * t + color2[1] * (1 - t);
	itm->square.b = color1[2] * t + color2[2] * (1 - t);
	lst->addItem(itm);

	if (engine->slow_time){
		itm = lst->getItem();
		itm->myType = solidcircle;
		itm->x = mX;
		itm->y = mY;
		itm->rot = 0;
		itm->zIndex = 0;
		itm->circle.radius = 20;
		itm->square.a = .3f;
		itm->square.r = .5f;
		itm->square.g = .5f;
		itm->square.b = .5f;
		lst->addItem(itm);
	}

}
void Player::renderProjectilePath(RenderList* list){
	if (!(*renderPathDetails->alive))return;
	float astep = (renderPathDetails->af - renderPathDetails->ai) / renderPathDetails->num;
	float tstep = (renderPathDetails->tf - renderPathDetails->ti) / renderPathDetails->num;
	float aInitial = renderPathDetails->ai;
	float tInitial = renderPathDetails->ti + renderPathDetails->toff;
	float xi = getBody()->position.x, yi = getBody()->position.y;
	vec2 dist = vec2(mX-xi,mY-yi);
	dist.normalize();
	dist *= PROJECTILESPEED + max(dist.dot(getBody()->velocity), 0);
	float xv = dist.x, yv = dist.y;
	for (int i = 0; i < renderPathDetails->num; i++){
		RenderItem* itm = list->getItem();
		itm->myType = solidsquare;
		itm->x = xi + xv*tInitial;
		itm->y = yi + yv*tInitial + renderPathDetails->half_gravity*tInitial*tInitial;
		itm->rot = tInitial;
		itm->square.a = aInitial;
		itm->square.r = renderPathDetails->r;
		itm->square.g = renderPathDetails->g;
		itm->square.b = renderPathDetails->b;
		itm->square.w = 1.f;
		itm->square.h = 1.f;
		list->addItem(itm);
		tInitial += tstep;
		aInitial += astep;
	}
}



void Player::dmg(int d){
	if (dmgfx <= 0){
		m_hp -= d;
		dmgfx = INVULNERABLETIME;
	}
}


void Player::fireMissile(GameEngine* e,float x, float y){
	createMissile = 1;
	missileTo.x = x;
	missileTo.y = y;
}

void Player::render(RenderList* lst){
	Mob::render(lst);
	if (mRenderProjectilePath){
		renderProjectilePath(lst);
	}
	renderPsychicBar(lst);
	renderHealthBar(lst);
	body* b = getBody();
	RenderItem* itm = lst->getItem();
	itm->myType = DrawType::texture;
	itm->x = -100;
	itm->y = 10;
	itm->rot = 0;
	itm->zIndex = 0;
	itm->tex.w = 100;
	itm->tex.h = 60;
	itm->tex.resID = 5;
	itm->tex.t1 = itm->tex.t2 = 1;
	itm->tex.s1 = itm->tex.s2 = 1;
	lst->addItem(itm);

	itm = lst->getItem();
	itm->myType = DrawType::texture;
	itm->x = 40;
	itm->y = 10;
	itm->rot = 0;
	itm->zIndex = 0;
	itm->tex.w = 100;
	itm->tex.h = 60;
	itm->tex.resID = 6;
	itm->tex.t1 = itm->tex.t2 = 1;
	itm->tex.s1 = itm->tex.s2 = 1;
	lst->addItem(itm);

	itm = lst->getItem();
	itm->myType = DrawType::texture;
	itm->x = 350;
	itm->y = 10;
	itm->rot = 0;
	itm->zIndex = 0;
	itm->tex.w = 100;
	itm->tex.h = 60;
	itm->tex.resID = 7;
	itm->tex.t1 = itm->tex.t2 = 1;
	itm->tex.s1 = itm->tex.s2 = 1;
	lst->addItem(itm);
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

void Player::fireGrappleHook(float x, float y){
	if (grappleHook == NULL){
		fireGrapplingHook = 1;
		grapplingHookTo.x = x;
		grapplingHookTo.y = y;
	}
}

void Player::fireSecondHook(float x, float y){
	if (grappleHook == NULL || grappleHook->otherHook != NULL)
		return;

	fire2ndGrapplingHook = 1;
	grapplingHookTo.x = x;
	grapplingHookTo.y = y;

	
}

void Player::releaseGrappleHook(){
	if (grappleHook != NULL){
		grappleHook->shouldreturn = true;
	}
}