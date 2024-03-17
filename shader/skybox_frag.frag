#version 460

layout (location = 0) out vec4 FragColor;
layout (binding = 2) uniform samplerCube skyBoxTexture;

in vec3 vertexPositionFrag;
in vec4 position;



void main() {
    vec3 colour = vec3(0.0);

	vec3 skyBoxTextureColour = texture(skyBoxTexture, normalize(vertexPositionFrag)).rgb;

	FragColor = vec4(skyBoxTextureColour, 1.0);
}