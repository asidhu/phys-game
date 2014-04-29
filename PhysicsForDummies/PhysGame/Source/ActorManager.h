#pragma once

class Actor;
class ActorManager{
public:

	//Gets or creates actor of this actorType? probably creates.
	virtual Actor* getActorByType(int actorType,float w, float h)=0;
	//Gets or creates actor of this unique ID
	virtual Actor* getActorByID(int actorID,float w, float h) = 0;


	virtual void handleNewActor(Actor*){}
	virtual void tick(float t){}
};