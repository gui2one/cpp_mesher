#version 460 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec3 color;
layout(location=3) in vec2 t_coords;

uniform vec3 u_objectColor;
uniform mat4 u_model;
uniform mat4 u_projection;
uniform mat4 u_view;

out vec2 f_texCoords;
out vec4 f_fragColor;
void main()
{
    f_texCoords = t_coords; // + vec2(0.0, 0.05);
    f_fragColor = vec4(color * u_objectColor, 1.0);
    vec3 fragPos = vec3(u_model * vec4(position, 1.0));
    gl_Position = u_projection * u_view * vec4(fragPos, 1.0);
}