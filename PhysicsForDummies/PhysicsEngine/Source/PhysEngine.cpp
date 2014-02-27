#include "PhysEngine.h"
#include "sort.h"
#include "shape.h"
#include <cmath>
#include "BodyDef.h"
#include "solver.h"
#define max(a,b) ((a>b)?a:b)
#define min(a,b) ((a<b)?a:b)
body* PhysEngine::buildBody(bodydef &def){
	body *b = new body();
	b->position = def.position;
	box* c = new box();
	c->halfwidth = def.width/2;
	c->halfheight = def.height/2;
	b->form = c;
	b->velocity = def.velocity;
	b->mass = def.mass;
	b->restitution = def.restitution;
	b->coeffFriction = def.coeffFriction;
	b->data = def.data;
	if (b->mass == 0.f)
		b->invMass = 0;
	else
		b->invMass = 1.f / b->mass;
	b->rotation = def.rotation;
	b->invMomentInertia = (3 * b->invMass / (c->halfwidth*c->halfwidth + c->halfheight*c->halfheight));
	//bodies.push_back(b);
	if (root.nextX != 0)
		root.nextX->prevX = b;
	b->nextX = root.nextX;
	b->prevX = &root;
	root.nextX = b;
	b->form->calcAABB(b, &(b->AABB));
	m_numBodies++;
	return b;
}

void PhysEngine::createBody(float x, float y, float r, float vx, float vy, float mass, float restitution){
	body *b = new body();
	b->position.x = x;
	b->position.y = y;
	circle* c = new circle();
	c->radius = r;
	b->form = c;
	b->velocity.x = vx;
	b->velocity.y = vy;
	b->mass = mass;
	b->restitution = restitution;
	b->coeffFriction = .8f;
	if (mass == 0)
		b->invMass = 0;
	else
		b->invMass = 1.f / mass;
	b->invMomentInertia = 2*b->invMass /(r*r);
	//bodies.push_back(b);
	if (root.nextX != 0)
		root.nextX->prevX = b;
	b->nextX = root.nextX;
	b->prevX = &root;
	root.nextX = b;
	b->form->calcAABB(b, &(b->AABB));
}
void PhysEngine::createBox(float x, float y, float hw,float hh, float vx, float vy, float mass, float restitution, float rotate){
	body *b = new body();
	b->position.x = x;
	b->position.y = y;
	box* c = new box();
	c->halfheight = hh;
	c->halfwidth = hw;
	b->form = c;
	b->velocity.x = vx;
	b->velocity.y = vy;
	b->mass = mass;
	b->restitution = restitution;
	b->coeffFriction = .001f;
	if (mass == 0.f)
		b->invMass = 0;
	else
		b->invMass = 1.f / mass;
	b->rotation = rotate;
	b->invMomentInertia = (3 *b->invMass / (hw*hw + hh*hh));
	//bodies.push_back(b);
	if (root.nextX != 0)
		root.nextX->prevX = b;
	b->nextX = root.nextX;
	b->prevX = &root;
	root.nextX = b;
	b->form->calcAABB(b, &(b->AABB));
}

void PhysEngine::setup(float g, float minX, float minY, float maxX, float maxY){
	gravity = g;
	mX = minX;
	mY = minY;
	this->maxX = maxX;
	this->maxY = maxY;
}

void PhysEngine::removeBody(body* b){
	b->prevX->nextX = b->nextX;
	if (b->nextX != NULL)
		b->nextX->prevX = b->prevX;
	m_numBodies--;
	delete b;
}

void PhysEngine::step(float time){
	//std::vector<body*>::iterator it = bodies.begin(), it2;
	solver s;
	int numContacts = 0;

	sortByPos(&root);
	//collisionlist collisions;
	//O(n) algorithm
	
	//solve contacts
	s.solveContacts(root.nextX, velocityIterations, positionIterations);

	body *ptr;

	//it = bodies.begin();
	ptr = root.nextX;
	while (ptr!=NULL){
		body* b = ptr;
		//b->impulse += b->accumulatedImpulse;
		//b->instantTorque += b->accumTorque;
		//b->accumulatedImpulse.x = b->accumulatedImpulse.y = 0;
		//b->accumTorque = 0;
		b->applyImpulse();
	//	b->impulse.y = 0;
		b->impulse.y=b->accumulatedImpulse.y = gravity*b->mass*time;
		b->position.x += b->velocity.x*time;
		b->position.y += b->velocity.y*time;
		b->rotation =  (b->rotation + b->angularVelocity*time);
		b->form->calcAABB(b, &(b->AABB));
		b->dampen();
		//n^2 algorithm for testing
		/*for (it2 = it+1; it2 != bodies.end(); it2++){
			body *ptr = b;
			body *ptr2 = *it2;
			contactdetails dets;
				if (ptr->AABB.overlap(ptr2->AABB) && shape::detectCollision(ptr,ptr2,dets) ){
					vec2 relV2 = ptr->velocity - ptr2->velocity;
					vec2 relV = ptr->velocityRelative2Point(dets.contactPoint[0]) - ptr2->velocityRelative2Point(dets.contactPoint[1]);
					float dot = relV.dot(dets.contactNormal);
					float sumInvMass = ptr->invMass + ptr2->invMass;
					float crossA = dets.contactPoint[0].crossZ(dets.contactNormal), crossB = dets.contactPoint[1].crossZ(dets.contactNormal);
					float momInertiaA = crossA*crossA/ptr->momentInertia,
						momInertiaB = crossB*crossB/ptr2->momentInertia;
					float restitution = 1 + fmaxf(ptr->restitution, ptr2->restitution);
					if (relV.dot(dets.contactNormal) > 0){
						vec2 impulse = (restitution*dot / (sumInvMass+momInertiaA+momInertiaB)) * dets.contactNormal;
						float impulseMag = impulse.length();
						ptr->impulse -= impulse;
						ptr->instantTorque -= crossA*impulseMag;
						ptr2->impulse += impulse;
						ptr2->instantTorque += crossB*impulseMag;
					}
				}
		}*/
		ptr = ptr->nextX;
	}
	//contact solver
	
	

}