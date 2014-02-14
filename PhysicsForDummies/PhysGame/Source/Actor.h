#pragma once
class body;
class RenderList;
class GameEngine;
class Actor{
private:
	int m_actorID;
	body* m_body;
public:
	//hack?
	Actor(int actorID, body* body){
		m_actorID = actorID;
		m_body = body;
	}

	virtual ~Actor(){}
	body* getBody(){
		return m_body;
	}

	virtual void render(RenderList*);

	virtual bool tick(GameEngine*){
		return false;
	}
	
};