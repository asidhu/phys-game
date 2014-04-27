#pragma once
class body;
class RenderList;
class GameWorld;
class Camera;
class Actor{
private:
	int m_actorID;
	body* m_body;
public:
	//hack?
	Actor(int actorID, body* body=0L){
		m_actorID = actorID;
		m_body = body;
	}
	bool cullActor(Camera* cam);
	virtual ~Actor(){}
	body* getBody(){
		return m_body;
	}

	virtual void render(RenderList*);

	virtual bool tick(float timestep,GameWorld*){
		return false;
	}
	
};