

class body;
class contactdetails;
namespace collider{
	
	bool collideBoxBox(body*, body*, contactdetails*);
	bool collideCircleCircle(body*, body*, contactdetails*);
	bool detectCollision(body*, body*, contactdetails* );
}