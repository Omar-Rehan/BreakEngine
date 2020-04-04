#include "pch.h"
#include "Model.h"


class CustomIOStream : public Assimp::IOStream {
private:
	File* m_pFile;
	friend class CustomIOSystem;

protected:
	CustomIOStream(FileManager* pFileManager, const char* fileName) {
		m_pFile = pFileManager->CreateNewFile();
		m_pFile->OpenRead(fileName);
	}

public:
	~CustomIOStream() {
		m_pFile->Close();
	}

	size_t Read(void* pvBuffer, size_t pSize, size_t pCount) {
		if (!m_pFile->Check()) {
			std::cout << "No file pointer\n";
			pvBuffer = nullptr;
			return NULL;
		}

		unsigned long long numOfReadBytes = NULL;
		m_pFile->Read(pvBuffer, pSize, pCount, &numOfReadBytes);

		return numOfReadBytes;
	}
	size_t Write(const void* pvBuffer, size_t pSize, size_t pCount) {
		if (!m_pFile->Check()) {
			std::cout << "No file pointer\n";
			return NULL;
		}

		return m_pFile->Write(pvBuffer, pCount);
	}

	size_t FileSize() const {
		if (!m_pFile->Check()) {
			std::cout << "No file pointer\n";
			return NULL;
		}

		return m_pFile->GetFileLength();
	}
	
	aiReturn Seek(size_t pOffset, aiOrigin pOrigin) {
		if (pOrigin == aiOrigin_SET && m_pFile->Seek(pOffset) != NULL)
			return aiReturn::aiReturn_SUCCESS;
		return aiReturn::aiReturn_FAILURE;
	}
	size_t Tell() const {
		return m_pFile->Tell();
	}
	void Flush() {
		m_pFile->Flush();
	}
};
class CustomIOSystem : public Assimp::IOSystem {
public:
	FileManager* m_pFileManager = nullptr;

public:
	CustomIOSystem() = default;
	~CustomIOSystem() = default;

	bool Exists(const char* fileName) const {
		return m_pFileManager->Exists(fileName);
	}
	char getOsSeparator() const {
		return '/';
	}

	Assimp::IOStream* Open(const char* fileName, const char* pMode) {
		return new CustomIOStream(m_pFileManager, fileName);
	}
	void Close(Assimp::IOStream* pFile) {
		pFile->~IOStream();
		delete pFile;
	}
};


Model::Model(const std::string& path, FileManager* pFileManager) {
	SetFolderDirectory(path);
	
	/// Load & Parse the model file
	Assimp::Importer importer;
	CustomIOSystem* ioSystem = new CustomIOSystem();
	ioSystem->m_pFileManager = pFileManager;
	importer.SetIOHandler(ioSystem);

	const aiScene* pSceneObject = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
	if (pSceneObject == nullptr || pSceneObject->mFlags & AI_SCENE_FLAGS_INCOMPLETE || pSceneObject->mRootNode == NULL) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}


	/// Initialize the model object
	ProcessNode(pSceneObject->mRootNode, pSceneObject, pFileManager); // Add meshes by recursively traversing the nodes
}

void Model::SetFolderDirectory(const std::string& path) {
	m_directory = path.substr(0, path.find_last_of('/') + 1);
}

void Model::ProcessNode(const aiNode* node, const aiScene* sceneObject, FileManager* pFileManager) {
	/// Create/Push mesh objects to the "m_meshes" vector
	for (unsigned int meshIndex = 0; meshIndex < node->mNumMeshes; meshIndex++) { // index in the (node's) meshes array

		/// Create the mesh object
		int globalMeshIndex = node->mMeshes[meshIndex]; // index in the (scene object's) meshes array
		aiMesh* pMesh = sceneObject->mMeshes[globalMeshIndex]; // mesh in the (scene object's) meshes array
		Mesh meshObject = Mesh(GetMeshVertices(pMesh), GetMeshIndices(pMesh)); /// Create a mesh object from OUR class

		/// Get the mesh's material from the (scene object's) materials array
		int materialIndex = pMesh->mMaterialIndex;
		if (materialIndex >= 0) {
			aiMaterial* pMaterial = sceneObject->mMaterials[materialIndex];

			/// Get the material's textures
			meshObject.AddTextureVector(GetMeshTextures(pMaterial, aiTextureType_AMBIENT, pFileManager));
			meshObject.AddTextureVector(GetMeshTextures(pMaterial, aiTextureType_DIFFUSE, pFileManager));
			meshObject.AddTextureVector(GetMeshTextures(pMaterial, aiTextureType_SPECULAR, pFileManager));
			meshObject.AddTextureVector(GetMeshTextures(pMaterial, aiTextureType_NORMALS, pFileManager));
			meshObject.AddTextureVector(GetMeshTextures(pMaterial, aiTextureType_HEIGHT, pFileManager));
		}

		m_meshes.push_back(std::move(meshObject));
	}

	/// Process the children nodes (recursive calls)
	for (unsigned int childIndex = 0; childIndex < node->mNumChildren; childIndex++) {
		aiNode* pChildNode = node->mChildren[childIndex];
		ProcessNode(pChildNode, sceneObject, pFileManager);
	}
}

std::vector<Vertex> Model::GetMeshVertices(aiMesh* pMesh) {
	std::vector<Vertex> vertices;
	for (unsigned int vertexIndex = 0; vertexIndex < pMesh->mNumVertices; vertexIndex++) {
		Vertex vertex;
		/// Get position
		{
			vertex.position.x = pMesh->mVertices[vertexIndex].x;
			vertex.position.y = pMesh->mVertices[vertexIndex].y;
			vertex.position.z = pMesh->mVertices[vertexIndex].z;
		}
		/// Get normals
		{
			vertex.normal.x = pMesh->mNormals[vertexIndex].x;
			vertex.normal.y = pMesh->mNormals[vertexIndex].y;
			vertex.normal.z = pMesh->mNormals[vertexIndex].z;
		}
		/// Get texture coordinates
		{
			if (pMesh->mTextureCoords[0] == NULL) {
				vertex.textureCoordinates = glm::vec2(0.0f);
			} else {
				vertex.textureCoordinates.x = pMesh->mTextureCoords[0][vertexIndex].x;
				vertex.textureCoordinates.y = pMesh->mTextureCoords[0][vertexIndex].y;
			}
		}
		vertices.push_back(std::move(vertex));
	}
	return std::move(vertices);
}
std::vector<unsigned int> Model::GetMeshIndices(aiMesh* pMesh) {
	std::vector<unsigned int> indices;
	for (unsigned int faceIndex = 0; faceIndex < pMesh->mNumFaces; faceIndex++) {
		aiFace face = pMesh->mFaces[faceIndex];
		for (unsigned int indexIndex = 0; indexIndex < face.mNumIndices; indexIndex++)
			indices.push_back(face.mIndices[indexIndex]);
	}
	return std::move(indices);
}
std::vector<Texture> Model::GetMeshTextures(aiMaterial* pMaterial, aiTextureType textureType, FileManager* pFileManager) {
	std::vector<Texture> textures;
	for (unsigned int textureIndex = 0; textureIndex < pMaterial->GetTextureCount(textureType); textureIndex++) {
		/// Get the texture name
		aiString path;
		pMaterial->GetTexture(textureType, textureIndex, &path);


		/// Translate the ai texture type into our enum image map type
		ImageMapType myTextureType = ImageMapType::IMT_NULL;
		switch (textureType) {
		case aiTextureType_AMBIENT:
			myTextureType = ImageMapType::IMT_AmbientMap;
			break;
		case aiTextureType_DIFFUSE:
			myTextureType = ImageMapType::IMT_DiffuseMap;
			break;
		case aiTextureType_SPECULAR:
			myTextureType = ImageMapType::IMT_SpecularMap;
			break;
		case aiTextureType_NORMALS:
			myTextureType = ImageMapType::IMT_NormalMap;
			break;
		case aiTextureType_HEIGHT:
			myTextureType = ImageMapType::IMT_HeightMap;
			break;
		default:
			std::cout << "NEW TYPE? " << textureType << std::endl;
		}


		/// Push the texture into our vector
		{
			bool bAlreadyLoaded = false;
			/// Check if this texture was loaded before
			for (Texture& loadedTexture : m_loadedTextures) {
				if (std::strcmp(loadedTexture.GetPath().c_str(), path.C_Str()) == NULL) { // match
					bAlreadyLoaded = true;
					textures.push_back(loadedTexture);
					break;
				}
			}

			/// If not, load the texture first
			if (!bAlreadyLoaded) {
				const std::string fullPath = m_directory + path.C_Str();

				/// Create a new texture and push it into the model's loaded textures
				m_loadedTextures.push_back(Texture(fullPath.c_str(), myTextureType, pFileManager));

				/// Push the texture into the mesh's textures vector
				textures.push_back(m_loadedTextures.back());
			}
		}
	}
	return std::move(textures);
}

void Model::Render(ShaderProgram& shaderProgram) const {
	for (auto& mesh : m_meshes) mesh.Render(shaderProgram);
}