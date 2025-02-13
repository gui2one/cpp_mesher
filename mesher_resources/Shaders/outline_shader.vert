#version 460 core
precision highp float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoords;

uniform mat4 u_projection;
uniform vec3 u_viewPos;
uniform mat4 u_view;
uniform mat4 u_model;

void main(){

    float width_mult = 0.005;

    vec3 WorldPos = vec3(u_model * vec4(aPos, 1.0));
    // float dist = length(u_viewPos - aPos);
    float dist = length(u_viewPos - WorldPos);
    vec3 normals = vec3(inverse(transpose(u_model)) * vec4(aNormal, 1.0));
    float outline_width = (dist) * width_mult;
 
    gl_Position =  u_projection * u_view * vec4(WorldPos + normalize(normals) * outline_width, 1.0);
}