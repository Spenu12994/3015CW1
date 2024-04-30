#version 460

in vec3 vertexPositionPass;//position
in vec3 vertexNormalPass;//normal
layout (location = 0) out vec4 FragColor;


uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;
uniform bool torchOn;

uniform struct SpotLightInfo{
    vec3 Position;
    vec3 La;
    vec3 L;
    vec3 Direction;
    float Exponent;
    float Radius;
}Spot;//spot light

uniform struct MaterialInfo{
    vec3 Ka;//ambient
    vec3 Kd;//diffuse
    vec3 Ks;//specular
    float Shininess;//spec shininiess factor
}Material;

vec3 phongSpotModel(vec3 position, vec3 n){ //phong lighting
    vec3 lightness;
    if(torchOn)
        lightness = Spot.L;
    else
        lightness = vec3(0.0f);

    vec3 diffuse=vec3(0), spec=vec3(0);
    vec3 ambient=Spot.La*Material.Ka;


    vec3 s=normalize(Spot.Position-position);

    float cosAng=dot(-s, normalize(Spot.Direction));
    float angle=acos(cosAng);
    float spotScale;

    if (angle>=0.0&&angle<Spot.Radius){
        spotScale=pow(cosAng,Spot.Exponent);
        float sDotN=max(dot(s,n),0.0);
        diffuse=Material.Kd*sDotN;
        if (sDotN>0.0){
            vec3 v=normalize(-position.xyz);
            vec3 h=normalize(v+s);
            spec=Material.Ks*pow(max(dot(h,n),0.0),Material.Shininess);
        }
    }

    // attenuation
    float distance    = length(Spot.Position - vertexPositionPass);
    float attenuation = 1.0 / (1.0f + 0.0009f * distance + 0.00032f * (distance * distance));    

    diffuse *= attenuation;
    spec *= attenuation;

    return ambient+spotScale*(diffuse+spec)*lightness;
}

void main() {
    vec3 n = normalize(NormalMatrix*vertexNormalPass);
    vec3 camCoords=(ModelViewMatrix*vec4(vertexPositionPass,1.0)).xyz;
 

    FragColor = vec4(phongSpotModel(vertexPositionPass,normalize(vertexNormalPass)), 1.0);
}