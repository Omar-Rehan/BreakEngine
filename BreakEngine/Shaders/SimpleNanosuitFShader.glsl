#version 330 core

in vec2 textureCoordinates;
out vec4 fragmentColor;

struct Material {float shininessValue; sampler2D diffuseTexture1, specularTexture1, heightTexture1;};
uniform Material material;


void main() {
    vec4 d = texture(material.diffuseTexture1, textureCoordinates);
    vec4 s = texture(material.specularTexture1, textureCoordinates);
    vec4 h = texture(material.heightTexture1, textureCoordinates);

    fragmentColor = d;
}