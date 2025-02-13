#version 300 es
precision highp float;

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 color;

uniform mat4 u_model;
uniform mat4 u_projection;
uniform mat4 u_view;

out vec3 f_Color;

void main(){

    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0);
    f_Color = color;
}