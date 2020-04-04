#pragma once
#include "Texture.h"
#include "Model.h"


class ResourceManager {
private:
	FileManager* m_pFileManager;

	std::map<std::string, Texture> m_texturesCache;
	std::map<std::string, Model> m_modelsCache;

public:
	void Initialize(FileManager* pFileManager);
	void Terminate();

	Texture GetTexture(const std::string& relativePath, ImageMapType imageType);
	Model GetModel(const std::string& relativePath);

	ResourceManager() = default;
	~ResourceManager() = default;
};