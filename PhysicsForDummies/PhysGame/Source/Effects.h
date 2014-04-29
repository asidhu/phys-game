#pragma once
class RenderList;
class Effect{
public:
	virtual bool tick(float timestep) = 0;
	virtual void render(RenderList*) = 0;
	virtual void release(){}
};