struct DirectionalLight
{
    float intensity;
    float shadowBias;

    vec3 color;
    vec3 position;
    vec3 direction;
    
    mat4 lightSpaceMatrix;
};

struct SpotLight
{
    float intensity;
    float shadowBias;  

    float angle;
  
    float near_plane;
    float far_plane;

    vec3 color;
    vec3 position;
    vec3 direction;
  
    mat4 lightSpaceMatrix;
};

struct PointLight
{
    float intensity;
    float shadowBias;  

  
    float near_plane;
    float far_plane;

    vec3 color;
    vec3 position;

};

// Define your array of lights
#define MAX_DIR_LIGHTS 8
#define MAX_SPOT_LIGHTS 8
#define MAX_POINT_LIGHTS 8

uniform int u_numDirectionalLights;
layout(std140, binding=0) uniform DirectionalLightBlock {
    DirectionalLight dir_lights[MAX_DIR_LIGHTS];
};
uniform sampler2D u_directionalShadowMaps[MAX_DIR_LIGHTS];

uniform int u_numSpotLights;
layout(std140, binding=1) uniform SpotLightsBlock {
    SpotLight spot_lights[MAX_SPOT_LIGHTS];
};
uniform sampler2D u_spotShadowMaps[MAX_SPOT_LIGHTS];


uniform int u_numPointLights;
layout(std140, binding=2) uniform PointLightsBlock {
    PointLight point_lights[MAX_POINT_LIGHTS];
};
uniform samplerCube u_pointShadowMaps[MAX_POINT_LIGHTS];

