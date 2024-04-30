#version 460

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform sampler2D noiseTexture;

uniform bool bloom;
uniform bool hdr;
uniform bool noiseBool;
uniform float exposure;

float luminance(vec3 colour){
    return dot(colour.rgb, vec3(0.2126,0.7152,0.0722));
}

void main()
{             


    const float gamma = 0.5;
    vec3 hdrColor = texture(scene, TexCoords).rgb;     
    vec4 noise = texture(noiseTexture, TexCoords);
    
    //hdrColor = mix(hdrColor.rgb,noise.rgb,noise.a * 5.0f);

    if(hdr)
    {
        vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
        if(bloom)
            hdrColor += bloomColor; // additive blending
        // tone mapping
        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
        // gamma correct      
        result = pow(result, vec3(1.0 / gamma));
        
        

       if(noiseBool){
        // tone mapping
        result = vec3(1.0) - exp(-hdrColor * (1+exposure*5));
        // gamma correct      
        result = pow(result, vec3(1.0 / gamma));
        float green = luminance(result);
        FragColor = vec4(0.0,green*clamp(noise.a + 0.25,0.0,1.0), 0.0,1.0);
       }
       else
       FragColor = vec4(result, 1.0);
    }
    else
    {
        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        
        FragColor = vec4(result, 1.0);
    }
}