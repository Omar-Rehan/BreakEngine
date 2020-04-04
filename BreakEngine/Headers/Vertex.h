#pragma once
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"

struct Vertex {
	glm::vec3 position, normal;
	glm::vec2 textureCoordinates;

	Vertex() {}
	Vertex(float posX, float posY, float posZ, float nrmX, float nrmY, float nrmZ, float txtX, float txtY) {
		position = glm::vec3(posX, posY, posZ);
		normal = glm::vec3(nrmX, nrmY, nrmZ);
		textureCoordinates = glm::vec2(txtX, txtY);
	}
};