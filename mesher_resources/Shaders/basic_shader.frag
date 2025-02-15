#version 460 core
precision highp float;

/*
    include external files using GLSL-Shader-Includes library ( in vendor/ )
*/
// clang-format off
#include incl/lights_uniforms.glsl
#include incl/basic_material.glsl

// clang-format on

uniform Material material;

// uniform sampler2D diffuseTexture;

uniform vec3 u_ColorID;

in vec3 f_FragPos;
in vec3 f_Normal;
in vec3 f_Color;
in vec2 f_TexCoords;

uniform vec3 u_viewPos;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 ColorID;

in vec4 f_dirLightFragPosLightSpace[MAX_DIR_LIGHTS];
in vec4 f_spotLightFragPosLightSpace[MAX_SPOT_LIGHTS];
in vec4 f_pointLightFragPosLightSpace[MAX_POINT_LIGHTS];

/* sampling directions for point_lights shadow map */
vec3 sampleOffsetDirections[20] = vec3[](
    vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1), vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1),
    vec3(-1, 1, -1), vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0), vec3(1, 0, 1), vec3(-1, 0, 1),
    vec3(1, 0, -1), vec3(-1, 0, -1), vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1));

vec3 getNormalFromMap() {
  vec3 normal_color = texture(material.normalTexture, f_TexCoords).xyz;
  float exponent = 1.0 / 2.2;
  normal_color = pow(normal_color, vec3(exponent));
  vec3 tangentNormal = normal_color * 2.0 - 1.0;
  tangentNormal.y *= -1.0; /* invert ?  */
  vec3 Q1 = dFdx(f_FragPos);
  vec3 Q2 = dFdy(f_FragPos);
  vec2 st1 = dFdx(f_TexCoords);
  vec2 st2 = dFdy(f_TexCoords);

  vec3 N = normalize(f_Normal);
  vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
  vec3 B = -normalize(cross(N, T));
  mat3 TBN = mat3(T, B, N);

  vec3 result = normalize(TBN * tangentNormal);

  return result;
}

float ComputeDirLightShadow(vec4 fragPosLightSpace, sampler2D shadowMap, int light_index) {
  vec3 lightDir = normalize(dir_lights[light_index].position - f_FragPos);

  // float bias = max(dir_lights[light_index].shadowBias * (1.0 - dot(f_Normal, lightDir)),
  // dir_lights[light_index].shadowBias);
  float bias = max(dir_lights[light_index].shadowBias * (1.0 - dot(getNormalFromMap(), lightDir)),
                   dir_lights[light_index].shadowBias);

  // perform perspective divide
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  // transform to [0,1] range
  projCoords = projCoords * 0.5 + 0.5;

  float shadow = 0.0;
  if (projCoords.z > 1.0 || projCoords.y > 1.0 || projCoords.y < 0.0 || projCoords.x > 1.0 || projCoords.x < 0.0) {
    shadow = 0.0;
  } else {
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    vec2 texture_sizef = vec2(textureSize(shadowMap, 0));
    vec2 texelSize = 1.0 / texture_sizef;
    for (int x = -1; x <= 1; ++x) {
      for (int y = -1; y <= 1; ++y) {
        float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
        // check whether current frag pos is in shadow
        shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
      }
    }

    shadow /= 9.0;
  }

  return shadow;
}

float ComputeSpotLightShadow(vec4 fragPosLightSpace, sampler2D shadowMap, int light_index, float near_plane,
                             float far_plane) {
  vec3 lightDir = normalize(spot_lights[light_index].position - f_FragPos);

  // float bias = max(spot_lights[light_index].shadowBias * (1.0 - dot(f_Normal, lightDir)),
  // spot_lights[light_index].shadowBias);
  float bias = max(spot_lights[light_index].shadowBias * (1.0 - dot(getNormalFromMap(), lightDir)),
                   spot_lights[light_index].shadowBias);

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

float ComputePointLightShadow(vec4 fragPos, samplerCube shadowMap, int light_index, float near_plane, float far_plane) {
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
  float bias = 0.05;
  int samples = 20;
  float viewDistance = length(u_viewPos - fragPos.xyz);
  float diskRadius = 0.01;
  for (int i = 0; i < samples; ++i) {
    float closestDepth = texture(shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
    closestDepth *= far_plane;  // undo mapping [0;1]
    if (currentDepth - bias > closestDepth) shadow += 1.0;
  }
  shadow /= float(samples);

  return shadow;
}
float lerp(float val, float src_min, float src_max, float dst_min, float dst_max) {
  return (val - src_min) / (src_max - src_min) * (dst_max - dst_min) + dst_min;
}

vec3 ComputeLighting() {
  vec3 final_color = vec3(0, 0, 0);
  vec3 color = texture(material.diffuseTexture, f_TexCoords).rgb * material.diffuseColor;
  // vec3 color = vec3(1.0,0,0);
  vec3 normal = getNormalFromMap();

  for (int i = 0; i < u_numDirectionalLights; i++) {
    // vec3 normal = normalize(f_Normal);

    vec3 lightColor = dir_lights[i].color;
    // ambient
    vec3 ambient = 0.0 * lightColor;
    // diffuse
    float diff = max(dot(dir_lights[i].direction, normal), 0.0);
    diff = diff * dir_lights[i].intensity;
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(u_viewPos - f_FragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(dir_lights[i].direction + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    specular *= dir_lights[i].intensity;
    // calculate shadow
    float shadow = ComputeDirLightShadow(f_dirLightFragPosLightSpace[i], u_directionalShadowMaps[i], i);
    vec3 lighting = (ambient + (1.0 - shadow) * ((diffuse * color) + specular));
    final_color += lighting;
  }

  for (int i = 0; i < u_numSpotLights; i++) {
    vec3 lightColor = spot_lights[i].color;
    // ambient
    vec3 ambient = 0.0 * lightColor;
    // diffuse
    vec3 light_dir = normalize(spot_lights[i].position - f_FragPos);
    float diff = max(dot(light_dir, normal), 0.0);
    diff = diff * spot_lights[i].intensity;
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(u_viewPos - f_FragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(light_dir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    specular *= spot_lights[i].intensity;
    // calculate shadow
    float shadow = ComputeSpotLightShadow(f_spotLightFragPosLightSpace[i], u_spotShadowMaps[i], i,
                                          spot_lights[i].near_plane, spot_lights[i].far_plane);
    vec3 lighting = (ambient + (1.0 - shadow) * ((diffuse * color) + specular));

    /* radius attenuation */
    // perform perspective divide
    vec3 projCoords = f_spotLightFragPosLightSpace[i].xyz / f_spotLightFragPosLightSpace[i].w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    float dist_from_center = distance(projCoords.xy, vec2(0.5, 0.5)) * 2.0;
    float radius_att = 1.0 - dist_from_center;
    radius_att = clamp(radius_att, 0.0, 1.0);

    /* distance attenuation */
    float dist_from_light = distance(f_FragPos, spot_lights[i].position);
    float dist_att = 1.0 / (dist_from_light * dist_from_light);
    final_color += lighting * radius_att * dist_att;
  }

  for (int i = 0; i < u_numPointLights; i++) {
    vec3 lightColor = point_lights[i].color;
    // ambient
    vec3 ambient = 0.0 * lightColor;
    // diffuse
    vec3 light_dir = normalize(point_lights[i].position - f_FragPos);
    float diff = max(dot(light_dir, normal), 0.0);
    diff = diff * point_lights[i].intensity;
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(u_viewPos - f_FragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(light_dir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    specular *= point_lights[i].intensity;
    // calculate shadow
    float shadow = ComputePointLightShadow(vec4(f_FragPos, 1.0), u_pointShadowMaps[i], i, point_lights[i].near_plane,
                                           point_lights[i].far_plane);
    vec3 lighting = (ambient + (1.0 - shadow) * ((diffuse * color) + specular));

    /* distance attenuation */
    float dist_from_light = distance(f_FragPos, point_lights[i].position);
    float dist_att = 1.0 / (dist_from_light * dist_from_light);

    final_color += f_Color * lighting * dist_att;
    // final_color += vec3(1.0, 0.0, 0.0);
  }

  return final_color;
}

void main() {
  vec3 lighting = ComputeLighting();
  FragColor = vec4(lighting, 1.0);
// FragColor = vec4(1.0, 0.0, 0.0, 1.0);
  ColorID = vec4(u_ColorID, 1.0);
  // ColorID = vec4(1.0, 0.0, 0.0, 1.0);
}