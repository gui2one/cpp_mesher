#version 300 es
precision highp float;

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 t_coords;

uniform mat4 u_model;
uniform mat4 u_projection;

out vec2 f_t_coords;
out vec3 f_color;
void main(){

    gl_Position =  u_projection * u_model * vec4(vec3(position, 0.0) , 1.0);
    f_t_coords = t_coords;
    f_color = color;
}