#include "pch.h"
#include "ResourceManager.h"


void ResourceManager::Initialize(FileManager* pFileManager) {
	m_pFileManager = pFileManager;
}
void ResourceManager::Terminate() {
	m_modelsCache.clear();
	m_texturesCache.clear();
}

Texture ResourceManager::GetTexture(const std::string& relativePath, ImageMapType imageType) {
	if (m_texturesCache.find(relativePath) == m_texturesCache.end())
		m_texturesCache[relativePath] = Texture(relativePath.c_str(), imageType);

	return m_texturesCache[relativePath];
};
Model ResourceManager::GetModel(const std::string& relativePath) {
	if (m_modelsCache.find(relativePath) == m_modelsCache.end())
		m_modelsCache[relativePath] = Model(relativePath, m_pFileManager);

	return m_modelsCache[relativePath];
}