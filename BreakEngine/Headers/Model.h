#pragma once
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/ZipArchiveIOSystem.h"
#include "Mesh.h"

#include "physfs.h"
#include "FileManager.h"


class Model {
public:
	Model() = default;
	Model(const std::string& modelFilePath, FileManager* pFileManager);
	void Render(ShaderProgram& shaderProgram) const;
	std::vector<Mesh> m_meshes;

private:
	std::string m_directory;

	std::vector<Texture> m_loadedTextures;

	void SetFolderDirectory(const std::string& path);
	void ProcessNode(const aiNode* node, const aiScene* sceneObject);

	std::vector<Vertex> GetMeshVertices(aiMesh* mesh);
	std::vector<unsigned int> GetMeshIndices(aiMesh* mesh);
	std::vector<Texture> GetMeshTextures(aiMaterial* material, aiTextureType textureType);
};