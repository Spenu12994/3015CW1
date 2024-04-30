#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 vertexNormalPass;
out vec3 vertexPositionPass;


uniform mat4 MVP;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 ModelMatrix;

void main()
{
    vertexNormalPass = mat3(transpose(inverse(ModelMatrix))) * VertexNormal;
    vertexPositionPass = vec3(ModelMatrix * vec4(VertexPosition, 1.0));
    gl_Position = MVP*vec4(VertexPosition,1.0);
}