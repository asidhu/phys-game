#pragma once
class body;
class RenderList;
class Actor{
private:
	int m_actorID;
	body* m_body;
public:
	//hack?
	bool onGround;
	Actor(int actorID, body* body){
		m_actorID = actorID;
		m_body = body;
		onGround = false;
	}

	body* getBody(){
		return m_body;
	}

	void render(RenderList*);

	bool tick(){
		return false;
	}
	
};