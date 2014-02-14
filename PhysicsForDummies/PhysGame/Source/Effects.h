#pragma once
class RenderList;
class Effect{
public:
	virtual bool tick() = 0;
	virtual void render(RenderList*) = 0;
};