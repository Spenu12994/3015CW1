#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

out vec3 vertexNormalPass;
out vec3 vertexPositionPass;
out vec2 TexCoord;

uniform mat4 MVP;
uniform mat4 ModelMatrix;


void main()
{
    vertexNormalPass = mat3(transpose(inverse(ModelMatrix))) * VertexNormal;
    vertexPositionPass = vec3(ModelMatrix * vec4(VertexPosition, 1.0));
    TexCoord = VertexTexCoord;
    gl_Position = MVP*vec4(VertexPosition,1.0);
}