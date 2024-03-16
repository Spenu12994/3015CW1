#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;




out vec3 LightIntensity;
out vec4 pos;
out vec3 norm;

uniform vec4 VertexLightPosition;
uniform vec3 Kd;
uniform vec3 Ld;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;


void GetCameraSpaceValues(out vec3 normal, out vec3 position){
    norm = normalize(NormalMatrix * VertexNormal);
    pos = (ModelViewMatrix * vec4(VertexPosition, 1.0));
}

void main()
{
    vec3 n = normalize(NormalMatrix*VertexNormal);
    vec3 camCoords=(ModelViewMatrix*vec4(VertexPosition,1.0)).xyz;

    vec3 lightPosToVertexPosDirection = normalize(vec3(VertexLightPosition - pos));
    LightIntensity = Kd * Ld * max(dot(lightPosToVertexPosDirection, norm), 0.0);

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
