#version 460

in vec3 vertexPositionPass;
in vec3 vertexNormalPass;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;
layout(binding=0) uniform sampler2D Tex1;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;



uniform struct LightInfo{
    vec4 Position;
    vec3 La;
    vec3 L;
}lights[3];//multi light

uniform struct MaterialInfo{
    vec3 Ka;//ambient
    vec3 Kd;//diffuse
    vec3 Ks;//specular
    float Shininess;//spec shininiess factor
}Material;


vec3 phongModel(int light, vec3 position, vec3 n){ //phong lighting
    vec3 texColor=texture(Tex1,TexCoord).rgb;
    vec3 ambient=lights[light].La*texColor;
    
    vec3 s=normalize(vec3(lights[light].Position.xyz-position));
    float sDotN=max(dot(s,n), 0.0);
    vec3 diffuse=texColor*sDotN;
    vec3 spec=vec3(0.0);
    if (sDotN>0.0){
        vec3 v=normalize(-position.xyz);
        vec3 r=reflect(-s,n);
        spec=Material.Ks*pow(max(dot(r,v),0.0),Material.Shininess);
    }
    return ambient+(diffuse+spec)*lights[light].L*texColor;
}


void main() {
    vec3 n = normalize(NormalMatrix*vertexNormalPass);
    vec3 camCoords=(ModelViewMatrix*vec4(vertexPositionPass,1.0)).xyz;

    vec3 colour;

    colour = vec3(0.0);
    for(int i=0; i<3;i++){
        colour += phongModel(i,camCoords,n);
    }


    FragColor = vec4(colour, 1.0);
}