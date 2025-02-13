#version 300 es
precision highp float;

layout (location = 0)in vec2 position;
layout (location = 1)in vec3 normal;
layout (location = 2)in vec2 t_coords;

uniform mat4 u_model;
uniform mat4 u_projection;
uniform float u_aspect;
uniform float u_view_width;

out vec2 vTextureCoord;

void main()
{
    vTextureCoord = t_coords;
    gl_Position = u_projection  * u_model * vec4(vec3(position.x, position.y, 1.0), 1.0);
}