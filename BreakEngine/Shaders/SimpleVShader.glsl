#version 330 core

layout (location = 0) in vec3 posAttrib;

void main() {
	gl_Position = vec4(posAttrib, 1.0f);
}