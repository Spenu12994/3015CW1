#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 vertexNormalPass;
out vec3 vertexPositionPass;


uniform mat4 MVP;

void main()
{
    vertexNormalPass = VertexNormal;
    vertexPositionPass = VertexPosition;
    gl_Position = MVP*vec4(VertexPosition,1.0);
}