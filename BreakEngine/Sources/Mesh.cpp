#include "pch.h"
#include "Mesh.h"

int Mesh::s_maxTextures = 0;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) : m_vertices(std::move(vertices)), m_indices(std::move(indices)) {
	if (s_maxTextures == NULL) glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &s_maxTextures);

	InitializeMesh();
}
void Mesh::InitializeMesh() {
	/// Vertex array object
	{
		glGenVertexArrays(1, &m_vaoID);
		glBindVertexArray(m_vaoID);
	}

	/// Vertex buffer object
	{
		glGenBuffers(1, &m_vboID);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
	}

	/// Element buffer object
	{
		glGenBuffers(1, &m_eboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
	}

	/// Vertex attributes
	{
		/// Postition attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));

		/// Normal attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));

		/// Texture coordinates attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, textureCoordinates)));
	}

	glBindVertexArray(NULL);
}

void Mesh::AddTexture(Texture texture) {
	if (m_textures.size() == s_maxTextures) return;
	m_textures.push_back(texture);
}
void Mesh::AddTextureVector(std::vector<Texture>&& newTextures) {
	m_textures.insert(m_textures.end(), newTextures.begin(), newTextures.end());
}
void Mesh::Render(ShaderProgram& shaderProgram) const {
	/// Setup sampler2D uniforms
	int numOfTextures = 0;
	int numOfAmbientMaps = 0, numOfDiffuseMaps = 0, numOfSpecularMaps = 0, numOfNormalMaps = 0, numOfHeightMaps = 0;
	for (const Texture& texture : m_textures) {
		
		/// Generate uniform name
		std::string uniformName = "material."; // MAKE SURE YOUR MODEL UNIFORM NAMES FOLLOW THIS STYLE
		switch (texture.GetImageMapType()) {
		case ImageMapType::IMT_AmbientMap:
			uniformName += "ambientTexture" + std::to_string(++numOfAmbientMaps);
			break;
		case ImageMapType::IMT_DiffuseMap:
			uniformName += "diffuseTexture" + std::to_string(++numOfDiffuseMaps);
			break;
		case ImageMapType::IMT_SpecularMap:
			uniformName += "specularTexture" + std::to_string(++numOfSpecularMaps);
			break;
		case ImageMapType::IMT_NormalMap:
			uniformName += "normalTexture" + std::to_string(++numOfNormalMaps);
			break;
		case ImageMapType::IMT_HeightMap:
			uniformName += "heightTexture" + std::to_string(++numOfHeightMaps);
			break;
		case ImageMapType::IMT_NULL:
			uniformName += "NULLTypeTexture";
			break;
		default:
			uniformName += "UnknownTypeTexture";
		}

		texture.Activate(shaderProgram, numOfTextures++, uniformName);
	}

	/// Bind vertex data & Draw mesh
	glBindVertexArray(m_vaoID);
	shaderProgram.Activate();

	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(NULL);
}
