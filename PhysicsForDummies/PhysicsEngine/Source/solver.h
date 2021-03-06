#include "Body.h"
#include "collision.h"
#define MAX_BODIES 5000
#define MAX_CONTACTS 8000
class solver{
public:
	contactdetails constraints[MAX_CONTACTS];
	vec2 pos[MAX_BODIES], vel[MAX_BODIES];
	float rot[MAX_BODIES], angV[MAX_BODIES];
	float iMass[MAX_BODIES];
	float iMom[MAX_BODIES];
	int nContacts,
		nBodies;
	void generateContacts(body* head);
	void solveContacts(body* head, int velIterations, int posIterations);
	void test(body*);
};