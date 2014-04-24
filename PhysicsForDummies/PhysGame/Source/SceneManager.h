#include <vector>


class Scene;
class SceneManager{
public:
	Scene* currentScene;
	std::vector<Scene*> allScenes;
	void loadScenes(char* file);
	Scene* setupScene(int sceneid);
	void handleMouseUpdate(float x, float y);
	bool handleMouseClick(int type, float x, float y);
};