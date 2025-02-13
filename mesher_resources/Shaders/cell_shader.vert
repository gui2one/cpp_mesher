#version 300 es

precision highp float;
layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec3 color;
layout(location=3) in vec2 t_coords;

uniform mat4 u_model;
uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_light_space_matrix;




out vec3 f_FragPos;
out vec3 f_Normal;
out vec2 f_TexCoords;
out vec4 f_FragPosLightSpace;



void main()
{

    f_FragPos = vec3(u_model * vec4(position, 1.0));
    f_TexCoords = t_coords;
    f_Normal = transpose(inverse(mat3(u_model))) * normal;
    f_FragPosLightSpace = u_light_space_matrix * vec4(f_FragPos, 1.0);

    gl_Position = u_projection * u_view * vec4(f_FragPos, 1.0);
}