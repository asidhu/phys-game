#pragma once

class body;
class RenderList;
class StaticFixture{
public:
	StaticFixture(body* b){
		m_body = b;
	}
	body* m_body;
	int m_zdist;
	virtual void render(RenderList*);
};