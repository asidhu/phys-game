#pragma once
#include <map>
#include <assert.h>
struct TextureResource{
	char filename[256];
	int id;
};

struct AudioResource{
	char filename[256];
	int id;
};

struct ScriptResource{
	char filename[256];
	int id;
};

struct SceneResource{
	char filename[256];
	int idx;
	int nLayers;
};
class PlatformGraphics;
class Scene;
class ResourceManager{
	std::map<int, TextureResource*> m_texture_resources;
	//std::map<int, AudioResource*> m_audio_resources;
	std::map<int, SceneResource*> m_scene_resources;
	PlatformGraphics * m_graphics;
public:
	//loads file, gets all scenes by idx, filename, and numlayers
	void loadScenes(const char* filename);
	//constructs a scene for you with correct number of layers
	Scene* getScene(int idx);
	void registerTexture(int id,const char* filename);
	void freeTextures();
	void setupManager(PlatformGraphics* graphics){ m_graphics = graphics; }
};