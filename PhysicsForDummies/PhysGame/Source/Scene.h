#pragma once
#include <list>
class RenderList;
class StaticFixture;
class PlatformGraphics;
class Actor;
class Scene{
private:
	RenderList* m_lists;
	std::list<Actor*> m_fixtures;
	int m_numlayers;
public:
	Scene(int numLayers);
	void addFixture(Actor* fix, int layer);
	RenderList* render(int);
	virtual void tick();
};
