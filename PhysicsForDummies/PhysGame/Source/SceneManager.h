#include <vector>


class Scene;
class SceneManager{
public:
	Scene* currentScene;
	std::vector<Scene*> allScenes;
	void loadScenes(char* file);
	Scene* setupScene(int sceneid);
};