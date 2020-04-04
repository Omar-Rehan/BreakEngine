#version 330 core

layout (location = 0) in vec3 posAttribute;
layout (location = 1) in vec3 nrmAttribute;
layout (location = 2) in vec2 texAttribute;

out vec2 textureCoordinates;

uniform mat4 modelMat, viewMat, projMat;

void main() {
	mat4 scaleMatrix = mat4(1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.1f * nrmAttribute.x, 0.1f * nrmAttribute.y, 0.1f * nrmAttribute.z, 1.0f);

	gl_Position = projMat * viewMat * modelMat * scaleMatrix * vec4(posAttribute, 1.0f);
	textureCoordinates = texAttribute;
}