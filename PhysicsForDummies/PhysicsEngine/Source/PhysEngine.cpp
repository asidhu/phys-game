#include "PhysEngine.h"
#include "sort.h"
#include "shape.h"
#include <cmath>
#include "BodyDef.h"
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
	contactdetails tempSol[1000];
	int numContacts = 0;

	sortByPos(&root);
	//collisionlist collisions;
	//O(n) algorithm
	
	body * ptr = root.nextX;
	while (ptr->nextX != 0){
		body * ptr2 = ptr->nextX;
		while (ptr2 != 0){
			if (ptr->AABB.right >= ptr2->AABB.left ){
				if ( ptr->AABB.overlap(ptr2->AABB) && shape::detectCollision(ptr, ptr2, (tempSol + numContacts))){
					if ( numContacts>=1000){
						ptr2 = ptr2->nextX;
						continue;
					}
					contactdetails *dets = tempSol + numContacts;
					dets->b1 = ptr;
					dets->b2 = ptr2;
					dets->normalImpulse = 0;
					float crossA = dets->contactPoint[0].crossZ(dets->contactNormal), crossB = dets->contactPoint[1].crossZ(dets->contactNormal);
					float momInertiaA = crossA*crossA*ptr->invMomentInertia,
						momInertiaB = crossB*crossB*ptr2->invMomentInertia;
					dets->effectiveMass =( ptr->invMass + ptr2->invMass +momInertiaA + momInertiaB);
					if (dets->effectiveMass != 0)
						dets->effectiveMass = 1 / dets->effectiveMass;
					bool a = true;
					if (ptr->pre_collide != NULL)
						a&=ptr->pre_collide(ptr, dets);
					if (ptr2->pre_collide != NULL)
						a&=ptr2->pre_collide(ptr2, dets);
					if(a)
						numContacts++;
				}
			}
			else
				break;
			ptr2 = ptr2->nextX;
		}
		ptr = ptr->nextX;
	}

	float max = 0;
	//contact solver
	for (int i = 0; i < velocityIterations; i++){
		for (int j = 0; j < numContacts; j++){
			contactdetails* cd = tempSol + j;
			ptr = cd->b1;
			body *ptr2 = cd->b2;
			vec2 relV = ptr->velocityRelative2PointAccum(cd->contactPoint[0]) - ptr2->velocityRelative2PointAccum(cd->contactPoint[1]);
			float crossA = cd->contactPoint[0].crossZ(cd->contactNormal), crossB = cd->contactPoint[1].crossZ(cd->contactNormal);
			float dot = relV.dot(cd->contactNormal);
			float lamda = dot * cd->effectiveMass;
			float test = max(cd->normalImpulse + lamda, 0);
			lamda = test - cd->normalImpulse;
			cd->normalImpulse = test;
			vec2 impulse = lamda * cd->contactNormal;
			float impulseMag = impulse.length();
			ptr->accumulatedImpulse -= impulse;
			ptr->accumTorque -= crossA*impulseMag;
			
			ptr2->accumulatedImpulse += impulse;
			ptr2->accumTorque += crossB*impulseMag;
			//const float slop = .002f;
			//const float percent = .1f;
			//vec2 correction = max(abs(cd->penetration) - slop, 0.0f)*percent* cd->contactNormal *(1.0f / cd->effectiveMass);
			//ptr->impulse -= correction;
			//ptr2->impulse += correction;
		}
	}
	for (int i = 0; i < positionIterations; i++){
		for (int j = 0; j < numContacts; j++){
			contactdetails* cd = tempSol + j;
			ptr = cd->b1;
			body *ptr2 = cd->b2;
			//if (shape::detectCollision(ptr, ptr2, cd)){
			const float slop = .0002f;
			const float percent = 3.f;
			float impulse = max(fabs(cd->penetration) - slop, 0.0f)*percent * cd->effectiveMass;
			vec2 correction = impulse*cd->contactNormal;
			float crossA = cd->contactPoint[0].crossZ(cd->contactNormal), crossB = cd->contactPoint[1].crossZ(cd->contactNormal);
			ptr->impulse -= correction;
			ptr->instantTorque -= crossA*impulse;
			ptr2->impulse += correction;
			ptr2->instantTorque += crossB*impulse;
			//}
		}
	}
	for (int j = 0; j < numContacts; j++){
		contactdetails* cd = tempSol + j;
		ptr = cd->b1;
		body *ptr2 = cd->b2;
		vec2 fric;
		fric.x = -cd->contactNormal.y;
		fric.y = cd->contactNormal.x;
		float maxForce = cd->normalImpulse*max(ptr->coeffFriction, ptr2->coeffFriction);
		float tangent = fric.dot(ptr2->accumulatedImpulse);
		float crossA = cd->contactPoint[0].crossZ(cd->contactNormal), crossB = cd->contactPoint[1].crossZ(cd->contactNormal);
		float forceA = (tangent + fric.dot(ptr->velocity)*ptr->mass);
		vec2 fricA = (forceA>0?1.f:-1.f)*(fric)*min(maxForce,fabs(forceA) );
		ptr->impulse -= cd->contactNormal*cd->normalImpulse + fricA;
		ptr->instantTorque -= crossA*cd->normalImpulse;
		if (fabs(ptr->instantTorque - ptr->accumTorque)<.001 && ptr->instantTorque!=0)
			ptr = ptr;
		if (ptr->mass != 0 && cd->normalImpulse > max)
			max = crossA*cd->normalImpulse;
		if (ptr->mass != 0 && cd->normalImpulse > 3)
			crossA = 1;

		float forceB = (tangent + fric.dot(ptr2->velocity)*ptr2->mass);
		vec2 fricB = (forceB>0 ? 1.f : -1.f)*(fric)*min(maxForce, fabs(forceB));
		ptr2->impulse += cd->contactNormal*cd->normalImpulse-fricB;
		ptr2->instantTorque += crossB*cd->normalImpulse;
		if (ptr->post_collide != NULL)
			ptr->post_collide(ptr, cd);
		if (ptr2->post_collide != NULL)
			ptr2->post_collide(ptr2, cd);
		if (fabs(ptr2->instantTorque - ptr2->accumTorque)<.001 && ptr2->instantTorque != 0)
			continue;
		if (ptr2->mass != 0 && crossB*cd->normalImpulse > max)
			max = crossB*cd->normalImpulse;
		if (ptr2->mass != 0 && crossB*cd->normalImpulse > 3)
			crossB = 1;
	}
	//it = bodies.begin();
	ptr = root.nextX;
	while (ptr!=NULL){
		body* b = ptr;
		//b->impulse += b->accumulatedImpulse;
		//b->instantTorque += b->accumTorque;
		b->accumulatedImpulse.x = b->accumulatedImpulse.y = 0;
		b->accumTorque = 0;
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