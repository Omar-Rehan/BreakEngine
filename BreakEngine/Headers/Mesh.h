#pragma once
#include "Vertex.h"
#include "Texture.h"


class Mesh {
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	void AddTexture(Texture texture);
	void AddTextureVector(std::vector<Texture>&& newTextures);
	void Render(ShaderProgram& shaderProgram) const;

	unsigned int m_vaoID, m_vboID, m_eboID;
	std::vector<unsigned int> m_indices;

private:
	std::vector<Vertex> m_vertices;

	static int s_maxTextures;
	std::vector<Texture> m_textures;

	void InitializeMesh();




public:
	/// Copy Semantics
	Mesh(const Mesh&) = default;
	Mesh& operator = (const Mesh&) = default;
	/// Move Semantics
	Mesh(Mesh&&) = default;
	Mesh& operator = (Mesh&&) = default;
};