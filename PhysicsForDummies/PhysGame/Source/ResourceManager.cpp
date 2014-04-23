#include "ResourceManager.h"
#include "PlatformDependent\PlatformGraphics.h"


void ResourceManager::registerTexture(int id, char* filename){
	assert(m_texture_resources.find(id) == m_texture_resources.end());
	m_graphics->loadImage(id, filename);
	TextureResource* tex = (TextureResource*)malloc(sizeof(TextureResource));
	strcpy(tex->filename, filename);
	m_texture_resources[id] = tex;
}

void ResourceManager::freeTextures(){
	std::map<int, TextureResource*>::iterator it = m_texture_resources.begin();
	for (; it != m_texture_resources.end(); it++){
		m_graphics->freeImage(it->first);
	}
	m_texture_resources.clear();
}
