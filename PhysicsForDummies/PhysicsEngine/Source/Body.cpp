#include "Body.h"
#include "shape.h"

void body::setMass(float m){
	mass = m;
	if (mass == 0.f)
		invMass = 0;
	else
		invMass = 1.f / mass;
	box* c = (box*)form;
	invMomentInertia = (3 * invMass / (c->halfwidth*c->halfwidth + c->halfheight*c->halfheight));
}