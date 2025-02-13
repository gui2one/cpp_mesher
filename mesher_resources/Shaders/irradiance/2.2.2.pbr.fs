#version 460 core
precision highp float;

/*
    include external files using GLSL-Shader-Includes library ( in vendor/ )
*/
#include ../incl/lights_uniforms.glsl


in vec2 f_TexCoords;
in vec3 f_FragPos;
in vec3 f_Normal;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 ColorID;



// material parameters
layout(binding = 3) uniform sampler2D albedoMap;
layout(binding = 4) uniform sampler2D normalMap;
layout(binding = 5) uniform sampler2D metallicMap;
layout(binding = 6) uniform sampler2D roughnessMap;
layout(binding = 7) uniform sampler2D aoMap;

uniform vec3 u_albedo;

uniform float envIntensity;


// IBL
layout(binding = 0) uniform samplerCube irradianceMap;
layout(binding = 1) uniform samplerCube prefilterMap;
layout(binding = 2) uniform sampler2D brdfLUT;

// lights

#define MAX_LIGHTS 256
uniform vec3 lightPositions[MAX_LIGHTS];
uniform vec3 lightColors[MAX_LIGHTS];
uniform float lightIntensities[MAX_LIGHTS];
uniform int numLights;

uniform vec3 u_viewPos;

uniform vec3 u_ColorID;


in vec4 f_dirLightFragPosLightSpace[MAX_DIR_LIGHTS];
in vec4 f_spotLightFragPosLightSpace[MAX_SPOT_LIGHTS];
in vec4 f_pointLightFragPosLightSpace[MAX_POINT_LIGHTS];

/* sampling directions for point_lights shadow map */
vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
); 


const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal 
// mapping the usual way for performance anways; I do plan make a note of this 
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap()
{
    vec3 normal_color = texture(normalMap, f_TexCoords).xyz;
    float exponent = 1.0/2.2;
    normal_color = pow(normal_color, vec3(exponent));    
    vec3 tangentNormal = normal_color * 2.0 - 1.0;
    tangentNormal.y *= -1.0; /* invert ?  */
    vec3 Q1  = dFdx(f_FragPos);
    vec3 Q2  = dFdy(f_FragPos);
    vec2 st1 = dFdx(f_TexCoords);
    vec2 st2 = dFdy(f_TexCoords);

    vec3 N   = normalize(f_Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   
// ----------------------------------------------------------------------------

float ComputeDirLightShadow(vec4 fragPosLightSpace, sampler2D shadowMap, int light_index)
{
    vec3 lightDir = normalize(dir_lights[light_index].position - f_FragPos);
    

    // float bias = max(dir_lights[light_index].shadowBias * (1.0 - dot(f_Normal, lightDir)), dir_lights[light_index].shadowBias);  
    float bias = max(dir_lights[light_index].shadowBias * (1.0 - dot(getNormalFromMap(), lightDir)), dir_lights[light_index].shadowBias);  

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    float shadow = 0.0;
    if(projCoords.z > 1.0 || projCoords.y > 1.0 || projCoords.y < 0.0 || projCoords.x > 1.0 || projCoords.x < 0.0){

        shadow = 0.0;
    }else{
        // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
        float closestDepth = texture(shadowMap, projCoords.xy).r; 

        // get depth of current fragment from light's perspective
        float currentDepth = projCoords.z;
        vec2 texture_sizef = vec2(textureSize(shadowMap, 0));
        vec2 texelSize = 1.0 / texture_sizef;
        for(int x = -1; x <= 1; ++x)
        {
            for(int y = -1; y <= 1; ++y)
            {
                float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
                // check whether current frag pos is in shadow
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
            }    
        }
        
        shadow /= 9.0;   
    }

    return shadow;
} 
// ----------------------------------------------------------------------------

float ComputeSpotLightShadow(vec4 fragPosLightSpace, sampler2D shadowMap, int light_index, float near_plane, float far_plane)
{
    vec3 lightDir = normalize(spot_lights[light_index].position - f_FragPos);

    // float bias = max(spot_lights[light_index].shadowBias * (1.0 - dot(f_Normal, lightDir)), spot_lights[light_index].shadowBias);
    float bias = max(spot_lights[light_index].shadowBias * (1.0 - dot(getNormalFromMap(), lightDir)), spot_lights[light_index].shadowBias);

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // Apply depth linearization to projCoords.z
    float linearDepth = 2.0 * near_plane / (far_plane + near_plane - projCoords.z * (far_plane - near_plane));
    projCoords.z = linearDepth;

    float shadow = 0.0;
    if (projCoords.z > 1.0 || projCoords.y > 1.0 || projCoords.y < 0.0 || projCoords.x > 1.0 || projCoords.x < 0.0) {
        shadow = 1.0;
    } else {
        // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
        float closestDepth = texture(shadowMap, projCoords.xy).r;



        // reverse the depth linearization process
        closestDepth = 2.0 * near_plane / (far_plane + near_plane - closestDepth * (far_plane - near_plane));

        // get depth of current fragment from light's perspective
        float currentDepth = projCoords.z;
        vec2 texture_sizef = vec2(textureSize(shadowMap, 0));
        vec2 texelSize = 1.0 / texture_sizef;
        for (int x = -1; x <= 1; ++x) {
            for (int y = -1; y <= 1; ++y) {
                float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
                // reverse the depth linearization process
                pcfDepth = 2.0 * near_plane / (far_plane + near_plane - pcfDepth * (far_plane - near_plane));

                // check whether current frag pos is in shadow
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
            }
        }

        shadow /= 9.0;
    }

    return shadow;
}
// ----------------------------------------------------------------------------

float ComputePointLightShadow(vec4 fragPos, samplerCube shadowMap, int light_index, float near_plane, float far_plane)
{
    
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos.xyz - point_lights[light_index].position;
    // use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(shadowMap, fragToLight.xyz).r;
    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= point_lights[light_index].far_plane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);

    // now test for shadows
    float shadow = 0.0;
    float bias   = 0.05;
    int samples  = 20;
    float viewDistance = length(u_viewPos - fragPos.xyz);
    float diskRadius = 0.01;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
shadow /= float(samples);  

    return shadow;
}
// ----------------------------------------------------------------------------



void main()
{		
    // material properties
    vec3 albedo = pow(texture(albedoMap , f_TexCoords).rgb * u_albedo, vec3(1.0));
    // albedo = pow(albedo, vec3(1.0/2.2));
    float metallic = texture(metallicMap, f_TexCoords).r;
    float roughness = texture(roughnessMap, f_TexCoords).r;

    roughness = pow(roughness, 1.0/2.2);
    float ao = texture(aoMap, f_TexCoords).r;
       
    // input lighting data
    vec3 N = getNormalFromMap();
    vec3 V = normalize(u_viewPos - f_FragPos);
    vec3 R = reflect(-V, N); 

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);

    float shadow_amount = 0.0;
    for(int i=0; i<u_numDirectionalLights; i++)
    {

        float shadow = ComputeDirLightShadow(f_dirLightFragPosLightSpace[i], u_directionalShadowMaps[i], i);
        // shadow_amount = max(shadow_amount, shadow);


        vec3 lightColor = dir_lights[i].color;

        // calculate per-light radiance
        vec3 L = dir_lights[i].direction;
        vec3 H = normalize(V + L);

        vec3 radiance = lightColor * dir_lights[i].intensity;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);    
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);        
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        
         // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	                
            
        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (1.0-shadow) * (  kD * albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again

    }

    for(int i=0; i<u_numSpotLights; i++)
    {
        /* radius attenuation */
        // perform perspective divide
        vec3 projCoords = f_spotLightFragPosLightSpace[i].xyz / f_spotLightFragPosLightSpace[i].w;
        // transform to [0,1] range
        projCoords = projCoords * 0.5 + 0.5;

        float dist_from_center = distance(projCoords.xy, vec2(0.5, 0.5)) * 2.0;
        float radius_att =  pow(1.0-dist_from_center, 3.0);
        radius_att = clamp(radius_att, 0.0, 1.0);



        float shadow = ComputeSpotLightShadow(f_spotLightFragPosLightSpace[i], u_spotShadowMaps[i], i, spot_lights[i].near_plane, spot_lights[i].far_plane);     
        // calculate per-light radiance
        vec3 L = normalize(spot_lights[i].position - f_FragPos);
        vec3 H = normalize(V + L);
        float distance = length(spot_lights[i].position - f_FragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = spot_lights[i].color * spot_lights[i].intensity * attenuation * radius_att;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);    
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);        
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        
         // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	                
            
        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        



        // add to outgoing radiance Lo
        Lo += (1.0-shadow) * (kD * albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again          
    
    
    
    }

    for(int i=0; i<u_numPointLights; i++)
    {

        float shadow = ComputePointLightShadow(vec4(f_FragPos,1.0), u_pointShadowMaps[i], i, point_lights[i].near_plane, point_lights[i].far_plane);
        // calculate per-light radiance
        vec3 L = normalize(point_lights[i].position - f_FragPos);
        vec3 H = normalize(V + L);
        float distance = length(point_lights[i].position - f_FragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = point_lights[i].color * point_lights[i].intensity * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);    
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);        
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        
         // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	                
            
        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (1.0-shadow) * (kD * albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
    
    // ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    
    vec3 irradiance = texture(irradianceMap, N).rgb * envIntensity;
    vec3 diffuse      = irradiance * albedo;
    
    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);
    specular *= envIntensity;

    vec3 ambient = (kD * diffuse + specular) * ao;
    
    vec3 color = ambient + Lo;

    // HDR tonemapping
    // color = color / (color + vec3(1.0));
    // gamma correct
    // color = pow(color, vec3(1.0/2.2)); 


    
    FragColor = vec4(color , 1.0);
    // FragColor = vec4(kD , 1.0);

    ColorID = vec4(u_ColorID, 1.0);
}