#version 460

layout(location = 0) in vec3 VertexPosition;

out vec3 Vec;

uniform mat4 MVP;

void main(){
	Vec=VertexPosition;
}
