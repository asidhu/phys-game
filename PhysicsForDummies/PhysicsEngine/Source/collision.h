#pragma once
class body;


class contactdetails{
public:
	body *b1, *b2;
	int id1, id2;

	float penetration;
	vec2 contactNormal;
	vec2 contactPoint[2];
	float normalImpulse;
	float effectiveMass;
};
