#include "solver.h"
#include "shape.h"


#define max(a,b) ((a>b)?a:b)
#define min(a,b) ((a<b)?a:b)



void solver::test(body* head){
	body* ptr = head;
	while (ptr->nextX != 0){
		if (ptr->idx != -1){
			int idx = ptr->idx;
			if (ptr->invMass == 0 && ptr->position != pos[idx])
				ptr = ptr;
			if (ptr->position != pos[idx] ||
				ptr->velocity != vel[idx] ||
				ptr->rotation != rot[idx] ||
				ptr->angularVelocity != angV[idx])
				ptr = ptr;

			if (ptr->invMass != iMass[idx])
				ptr = ptr;
			if (ptr->position.x == INFINITY || ptr->velocity.x == INFINITY)
				ptr = ptr;
			//ptr->idx = -1;
		}
		ptr = ptr->nextX;
	}

}
void solver::solveContacts(body* head, int velIterations, int posIterations){
	nContacts = 0;
	nBodies = 0;
	body * ptr = head;
	while (ptr->nextX != 0){
		body * ptr2 = ptr->nextX;
		while (ptr2 != 0){
			if (nContacts >= MAX_CONTACTS || nBodies >= MAX_BODIES){
				ptr2 = ptr2->nextX;
				continue;
			}
			if (ptr->AABB.right >= ptr2->AABB.left){
				if (ptr->AABB.overlap(ptr2->AABB) && shape::detectCollision(ptr, ptr2, (constraints + nContacts))){
					
					contactdetails *dets = constraints + nContacts;
					dets->b1 = ptr;
					dets->b2 = ptr2;
					dets->normalImpulse = 0;
					float crossA = dets->contactPoint[0].crossZ(dets->contactNormal), crossB = dets->contactPoint[1].crossZ(dets->contactNormal);
					float momInertiaA = crossA*crossA*ptr->invMomentInertia,
						momInertiaB = crossB*crossB*ptr2->invMomentInertia;
					dets->effectiveMass = (ptr->invMass + ptr2->invMass + momInertiaA + momInertiaB);
					if (dets->effectiveMass != 0)
						dets->effectiveMass = 1 / dets->effectiveMass;
					bool a = true;
					if (ptr->pre_collide != NULL)
						a &= ptr->pre_collide(ptr, dets);
					if (ptr2->pre_collide != NULL)
						a &= ptr2->pre_collide(ptr2, dets);
					if (a)
						nContacts++;
					else{
						ptr2 = ptr2->nextX;
						continue;
					}
					if (ptr->idx == -1)
					{
						ptr->idx = nBodies;
						pos[nBodies] = ptr->position;
						vel[nBodies] = ptr->velocity;
						rot[nBodies] = ptr->rotation;
						angV[nBodies] = ptr->angularVelocity;
						iMass[nBodies] = ptr->invMass;
						iMom[nBodies] = ptr->invMomentInertia;
						nBodies++;
					}
					if (ptr2->idx == -1)
					{
						ptr2->idx = nBodies;
						pos[nBodies] = ptr2->position;
						vel[nBodies] = ptr2->velocity;
						rot[nBodies] = ptr2->rotation;
						angV[nBodies] = ptr2->angularVelocity;
						iMass[nBodies] = ptr2->invMass;
						iMom[nBodies] = ptr2->invMomentInertia;
						nBodies++;
					}
					dets->id1 = ptr->idx;
					dets->id2 = ptr2->idx;
				}
			}
			else
				break;
			ptr2 = ptr2->nextX;
		}
		ptr = ptr->nextX;
	}
	


	//contact solver
	for (int i = 0; i < velIterations; i++){
		for (int j = 0; j < nContacts; j++){
			contactdetails* cd = constraints + j;
			int  bid1 = cd->id1, bid2 = cd->id2;
			vec2 p1 = pos[bid1], p2 = pos[bid2];
			vec2 v1 = vel[bid1], v2 = vel[bid2];
			float rot1 = rot[bid1], rot2 = rot[bid2];
			float w1 = angV[bid1], w2 = angV[bid2];
			vec2 b1V = vec2(cd->contactPoint[0].y*-w1, cd->contactPoint[0].x*w1) + v1, b2V = vec2(cd->contactPoint[1].y*-w2, cd->contactPoint[1].x*w2) + v2;
			vec2 relV = b1V - b2V;
			float crossA = cd->contactPoint[0].crossZ(cd->contactNormal), crossB = cd->contactPoint[1].crossZ(cd->contactNormal);
			float dot = relV.dot(cd->contactNormal);
			float lamda = dot * cd->effectiveMass;
			float test = max(cd->normalImpulse + lamda, 0);
			lamda = test - cd->normalImpulse;
			cd->normalImpulse = test;
			vec2 impulse = lamda * cd->contactNormal;
			v1  -= impulse*iMass[bid1];
			w1 -= crossA*lamda*iMom[bid1];
			v2 += impulse*iMass[bid2];
			w2 += crossB*lamda*iMom[bid2];
			vel[bid1] = v1;
			angV[bid1] = w1;
			vel[bid2] = v2;
			angV[bid2] = w2;
			if ((iMass[bid1] == 0 || iMass[bid2] == 0) && vel[bid1].lengthSq()!=0 && vel[bid2].lengthSq()!=0)
				test = test;
			//const float slop = .002f;
			//const float percent = .1f;
			//vec2 correction = max(abs(cd->penetration) - slop, 0.0f)*percent* cd->contactNormal *(1.0f / cd->effectiveMass);
			//ptr->impulse -= correction;
			//ptr2->impulse += correction;
		}
	}
	
	for (int i = 0; i < posIterations; i++){
		for (int j = 0; j < nContacts; j++){
			contactdetails* cd = constraints + j;
			int bid1 = cd->id1, bid2 = cd->id2;
			//if (shape::detectCollision(ptr, ptr2, cd)){
			const float slop = .01f;
			const float percent = .4f;
			float impulse = max(fabs(cd->penetration) - slop, 0.0f)*percent * cd->effectiveMass;
			//vec2 correction = impulse*cd->contactNormal;
			float crossA = cd->contactPoint[0].crossZ(cd->contactNormal), crossB = cd->contactPoint[1].crossZ(cd->contactNormal);
			const float maxdist = .01f;
			const float maxtheta = .001f;
			pos[bid1] -= cd->contactNormal*min(maxdist, max(-maxdist, impulse*iMass[bid1]));
			rot[bid1] -= crossA*min(maxtheta, max(-maxtheta, impulse*iMom[bid1]));
			pos[bid2] += cd->contactNormal*min(maxdist, max(-maxdist, impulse*iMass[bid2]));
			rot[bid2] += crossB*min(maxtheta, max(-maxtheta, impulse*iMom[bid2]));
			//ptr->impulse -= correction;
			//ptr->instantTorque -= crossA*impulse;
			//ptr2->impulse += correction;
			//ptr2->instantTorque += crossB*impulse;
			//}
		}
	}
	
	for (int j = 0; j < nContacts; j++){
		contactdetails* cd = constraints + j;
		ptr = cd->b1;
		body *ptr2 = cd->b2;
		vec2 fric;
		fric.x = -cd->contactNormal.y;
		fric.y = cd->contactNormal.x;
		float maxForce = cd->normalImpulse*max(ptr->coeffFriction, ptr2->coeffFriction);
		float tangent = fric.dot(ptr2->accumulatedImpulse);
		float crossA = cd->contactPoint[0].crossZ(cd->contactNormal), crossB = cd->contactPoint[1].crossZ(cd->contactNormal);
		float forceA = (tangent + fric.dot(ptr->velocity)*ptr->mass);
		vec2 fricA = (forceA>0 ? 1.f : -1.f)*(fric)*min(maxForce, fabs(forceA));
		ptr->impulse -=  fricA;
		ptr->instantTorque -= crossA*cd->normalImpulse;
		float forceB = (tangent + fric.dot(ptr2->velocity)*ptr2->mass);
		vec2 fricB = (forceB>0 ? 1.f : -1.f)*(fric)*min(maxForce, fabs(forceB));
		ptr2->impulse -=  fricB;
		ptr2->instantTorque += crossB*cd->normalImpulse;
		if (ptr->post_collide != NULL)
			ptr->post_collide(ptr, cd);
		if (ptr2->post_collide != NULL)
			ptr2->post_collide(ptr2, cd);
	}
	ptr = head;
	while (ptr != 0){
		if (ptr->idx != -1){
			int idx = ptr->idx;
			if (ptr->invMass == 0 && ptr->position != pos[idx])
				ptr = ptr;
			ptr->position = pos[idx];
			ptr->velocity = vel[idx];
			ptr->rotation = rot[idx];
			ptr->angularVelocity = angV[idx];
			
			if (ptr->invMass != iMass[idx])
 				ptr = ptr;
			if (ptr->position.x == INFINITY || ptr->velocity.x ==INFINITY )
				ptr = ptr;
			ptr->idx = -1;
		}
		ptr = ptr->nextX;
	}
}