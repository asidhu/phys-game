#include <vector>


class Scene;
class LuaEngine;
class ResourceManager;
class GameWorld;
class SceneManager{
	LuaEngine*	m_lua_engine;
	Scene* currentScene;
	ResourceManager* m_res_manager;
	GameWorld* m_game_world;
public:
	void setupResources(ResourceManager* r){ m_res_manager = r; }
	Scene* getCurrentScene(){	return currentScene;}
	LuaEngine* getLuaEngine(){ return m_lua_engine; }
	GameWorld* getGameWorld(){ return m_game_world; }
	void setGameWorld(GameWorld* w){ m_game_world = w; }
	void tick(float timestep);
	Scene* setupScene(int sceneid);
	SceneManager();
	void handleMouseUpdate(float x, float y);
	bool handleMouseClick(int type, float x, float y);
};