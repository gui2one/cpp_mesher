#version 300 es
precision highp float;


uniform sampler2D diffuseTexture; 
uniform sampler2D shadowMap; 

in vec3 f_FragPos;
in vec3 f_Normal;
in vec2 f_TexCoords;
in vec4 f_FragPosLightSpace;


uniform vec3 u_viewPos;
uniform vec3 u_lightPos;
uniform vec3 u_lightDir;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 ColorID; 

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 lightDir = normalize(u_lightPos - f_FragPos);
    // vec3 lightDir = normalize(u_lightPos - f_FragPos);
    float bias = max(0.005 * (1.0 - dot(f_Normal, lightDir)), 0.005);  
    // float bias = 0.005;  
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;


    float shadow = 0.0;
    if(projCoords.z > 1.0 || projCoords.y > 1.0 || projCoords.y < 0.0 || projCoords.x > 1.0 || projCoords.x < 0.0){
    // if(true){

        shadow = 0.0;
    }else{
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;


    vec2 texture_sizef = vec2(textureSize(shadowMap, 0));
    vec2 texelSize = 1.0 / texture_sizef;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;   
    }

    return shadow;
} 

float lerp(float val, float src_min, float src_max, float dst_min, float dst_max){

    return (val - src_min) / (src_max - src_min) * (dst_max - dst_min) + dst_min; 
}

void main() {


    vec3 color = texture(diffuseTexture, f_TexCoords).rgb;
    vec3 normal = normalize(f_Normal);
    vec3 lightColor = vec3(1.0);
    vec3 viewDir = normalize(u_viewPos - f_FragPos);
    vec3 halfwayDir = normalize(u_lightDir + viewDir);  
    // ambient
    vec3 ambient = 0.1 * lightColor;
    // diffuse
    float dotP = max(dot(halfwayDir, normal), 0.0);
    float diff = 1.0;
    if(dotP < 0.2 ) diff = 0.0; 
    else if(dotP > 0.2 && dotP < 0.5) diff = 0.8; 
    else if(dotP > 0.5 && dotP < 1.0) diff = 1.0; 

    
    vec3 diffuse = diff * lightColor;
    // specular
    float spec = 0.0;
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow
    float shadow = ShadowCalculation(f_FragPosLightSpace);       
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);
    ColorID = vec4(0.0, 0.0, 0.0, 1.0);
    
}