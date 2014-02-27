#include "Body.h"
#include "collision.h"
class solver{
public:
	contactdetails constraints[1000];
	vec2 pos[500], vel[500];
	float rot[500], angV[500];
	float iMass[500];
	float iMom[500];
	int nContacts,
		nBodies;
	void solveContacts(body* head, int velIterations, int posIterations);
	void test(body*);
};