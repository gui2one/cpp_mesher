#version 460 core
precision highp float;

in vec3 position;
in vec3 normal;
in vec3 color;
in vec2 t_coords;

uniform mat4 u_projection;

out vec2 vTextureCoord;

void main()
{
    vTextureCoord = t_coords;
    gl_Position = u_projection * vec4(vec3(t_coords.x, t_coords.y, 1.0), 1.0);
}