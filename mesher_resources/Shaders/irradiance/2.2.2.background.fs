#version 460 core
precision highp float;

out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;
uniform float envIntensity;
void main()
{		
    vec3 envColor = textureLod(environmentMap, WorldPos, 0.0).rgb;
    
    envColor *= envIntensity;
    // HDR tonemap and gamma correct
    envColor = envColor / (envColor + vec3(1.0));
    
    envColor = pow(envColor, vec3(1.0/2.2)); 
    // envColor *= pow(envIntensity,1.0/2.2);
    FragColor = vec4(envColor, 1.0);
}