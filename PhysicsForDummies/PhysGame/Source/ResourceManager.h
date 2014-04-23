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
class PlatformGraphics;

class ResourceManager{
	std::map<int, TextureResource*> m_texture_resources;
	//std::map<int, AudioResource*> m_audio_resources;
	//std::map<int, ScriptResource*> m_script_resources;
	PlatformGraphics * m_graphics;
public:
	void registerTexture(int id, char* filename);
	void freeTextures();
	void setupManager(PlatformGraphics* graphics){ m_graphics = graphics; }
};