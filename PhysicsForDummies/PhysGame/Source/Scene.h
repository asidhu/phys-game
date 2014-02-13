#pragma once
#include <list>
class RenderList;
class StaticFixture;
class PlatformGraphics;
class Scene{
private:
	RenderList* m_lists;
	std::list<StaticFixture*> m_fixtures;
	int m_numlayers;
public:
	Scene(int numLayers);
	void addFixture(StaticFixture* fix, int layer);
	RenderList* render(int);
};
