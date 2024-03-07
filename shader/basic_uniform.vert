#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

uniform vec4 LightPosition;

uniform vec3 Kd;
uniform vec3 Ld;

uniform mat4 ModelViewMatrix;
uniform mat4 MVP;

uniform mat3 NormalMatrix;

void main()
{
    vec3 n = normalize(NormalMatrix * VertexNormal);
    vec4 pos=ModelViewMatrix*vec4(VertexPosition,1.0);
    vec3 s=normalize(vec3(LightPosition-pos));
    float sDotN=max(dot(s,n), 0.0);
    vec3 diffuse=Ld*Kd*sDotN;
    LightIntensity=diffuse;
    gl_Position = vec4(VertexPosition,1.0);
}
