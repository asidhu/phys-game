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
	contacts = new solver();
}

void PhysEngine::removeBody(body* b){
	b->prevX->nextX = b->nextX;
	if (b->nextX != NULL)
		b->nextX->prevX = b->prevX;
	m_numBodies--;
	delete b;
}

void PhysEngine::step(float time){

	int numContacts = 0;

	sortByPos(&root);
	//O(n) algorithm
	
	//solve contacts
	contacts->solveContacts(root.nextX, velocityIterations, positionIterations);

	body *ptr;

	//it = bodies.begin();
	ptr = root.nextX;
	while (ptr!=NULL){
		body* b = ptr;
		b->applyImpulse();
		b->impulse.y=b->accumulatedImpulse.y = gravity*b->mass*time;
		b->position.x += b->velocity.x*time;
		b->position.y += b->velocity.y*time;
		b->rotation =  (b->rotation + b->angularVelocity*time);
		b->form->calcAABB(b, &(b->AABB));
		b->dampen();
		ptr = ptr->nextX;
	}
	//contact solver
	
	

}