#pragma once
#include "PhysicsEngine\Source\Body.h"
#include "Grapple.h"
#define OKGRAPPLE 0x00F00000
class Grapplable{
public:
	Grapple* m_grapple;
	Grapplable(body* b){
		b->dataFlag |= OKGRAPPLE;
		m_grapple = NULL;
	}
	void grapple(Grapple* m){
		//m_grapple = m;
	}
	void release(){
		m_grapple = NULL;
	}
	~Grapplable(){
		if(m_grapple!=NULL)
			m_grapple->release();
	}
};