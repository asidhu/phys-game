#include <vector>


class Scene;
class SceneManager{
	Scene* currentScene;
	std::vector<Scene*> allScenes;
	void loadScenes(char* file);
	Scene* setupScene(int sceneid);
};