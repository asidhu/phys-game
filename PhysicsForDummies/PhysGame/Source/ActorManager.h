#pragma once

class Actor;
class ActorManager{
public:

	//Gets or creates actor of this actorType? probably creates.
	virtual Actor* getActorByType(int actorType)=0;
	//Gets or creates actor of this unique ID
	virtual Actor* getActorByID(int actorID) = 0;

};