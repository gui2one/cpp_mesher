#version 460 core
precision highp float;

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec3 color;
layout(location=3) in vec2 t_coords;

uniform mat4 u_lightSpaceMatrix;
uniform mat4 u_model;

out vec2 f_tCoords;

void main()
{
    gl_Position = u_lightSpaceMatrix * u_model * vec4(position, 1.0);
    f_tCoords = t_coords;
} 