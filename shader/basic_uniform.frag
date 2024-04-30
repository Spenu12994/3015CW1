#version 460

in vec3 vertexPositionPass;
in vec3 vertexNormalPass;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

layout (binding = 1) uniform sampler2D cementTex;
layout (binding = 2) uniform sampler2D mossTex;
layout (binding = 3) uniform sampler2D noiseTex;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

uniform bool disint;


uniform struct LightInfo{
    vec4 Position;
    vec3 La;
    vec3 L;
}lights[4];//multi light

uniform struct MaterialInfo{
    vec3 Ka;//ambient
    vec3 Kd;//diffuse
    vec3 Ks;//specular
    float Shininess;//spec shininiess factor
}Material;


vec3 phongModel(int light, vec3 position, vec3 n){ //phong lighting


    vec4 cementTexColour=texture(cementTex,TexCoord);
    vec4 mossTexColour=texture(mossTex,TexCoord);
    vec3 texColour=mix(cementTexColour.rgb,mossTexColour.rgb,mossTexColour.a);//because moss is a png we add the alpha

    vec3 ambient=lights[light].La*texColour;
    
    vec3 s=normalize(vec3(lights[light].Position.xyz-position));
    float sDotN=max(dot(s,n), 0.0);
    vec3 diffuse=texColour*sDotN;
    vec3 spec=vec3(0.0);
    if (sDotN>0.0){
        vec3 v=normalize(-position.xyz);
        vec3 r=reflect(-s,n);
        spec=Material.Ks*pow(max(dot(r,v),0.0),Material.Shininess);
    }


    return ambient+(diffuse+spec)*lights[light].L*texColour;
}


void main() {
    vec3 n = normalize(NormalMatrix*vertexNormalPass);
    vec3 camCoords=(ModelViewMatrix*vec4(vertexPositionPass,1.0)).xyz;

    vec3 colour;

    float noiseLow = 0.4f;
    float noiseHigh = 0.6f;
    
    vec4 noise = texture(noiseTex, TexCoord);

    colour = vec3(0.0);


    
    for(int i=0; i<4;i++){
        colour += phongModel(i,camCoords,n);
    }

    
    float brightness = dot(colour, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(colour, 1.0);
    else
        BrightColor = vec4(0.0,0.0,0.0,1.0);

    
    if(noise.a < noiseLow){
    if(disint){
        discard;
    }
    else{
        colour = vec3(0.5,0.4,1.0);
    }
    }

    if(noise.a > noiseHigh){
    if(disint){
        discard;

        }
    else{
        colour = vec3(0.5,0.4,1.0);
    }
    }

    FragColor = vec4(colour, 1.0);
}