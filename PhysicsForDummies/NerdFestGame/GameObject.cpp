#include "GameObjects.h"
#include "Grapple.h"
#include "PhysicsEngine\Source\Body.h"

GameObject::GameObject(body* b, bool grapple, bool missile):Actor(1,b){
	m_obj = new autoptr<GameObject>(this);
	m_obj->get();
	b->data = this;
	if (grapple)
		b->dataFlag |= OKGRAPPLE;
	if (missile)
		b->dataFlag |= OKMISSILE;
	m_grapple = NULL;
}
void GameObject::grapple(Grapple* m){
	m_grapple = m;
}
void GameObject::release(){
	m_grapple = NULL;
}
GameObject::~GameObject(){
	m_obj->kill();
	m_obj->free();
}