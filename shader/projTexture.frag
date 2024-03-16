#version 460

in vec4 pos;
in vec3 norm;
in vec2 textureCoordinates;
in vec3 vertexPositionFrag;
in vec4 projectedTextureCoordinates;


layout (location = 0) out vec4 FragColor;


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


vec3 phongModel(int light, vec3 cameraNorm, vec4 position){ //phong lighting
    vec3 ambient=lights[light].La*Material.Ka;
    vec3 s = normalize(vec3(lights[light].Position - (position * lights[light].Position)));
    float sDotN=max(dot(s,cameraNorm), 0.0);
    vec3 diffuse=Material.Kd*sDotN;
    vec3 spec=vec3(0.0);
    if (sDotN>0.0){
        vec3 v=normalize(-position.xyz);
        vec3 r=reflect(-s,cameraNorm);
        spec=Material.Ks*pow(max(dot(r,v),0.0),Material.Shininess);
    }
    return ambient+(diffuse+spec)*lights[light].L;
}

void main() {

    vec3 colour = vec3(0.0);
    vec3 normalised = normalize(norm);
    
    for(int i=0; i<3;i++){
        colour += phongModel(i, norm, pos);
    }

    FragColor = vec4(colour,1);
}
