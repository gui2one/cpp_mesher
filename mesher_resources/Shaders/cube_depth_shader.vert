#version 460 core
precision highp float;

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec3 color;
layout(location=3) in vec2 t_coords;

uniform mat4 u_model;

uniform sampler2D opacity_texture;

out vec2 f_tCoords;
// out float f_opacity;


// float threshold(vec3 clr)
// {
//     vec3 clr_dir = normalize(clr);
//     vec3 white_dir = normalize(vec3(1.0, 1.0, 1.0));
//     float dotP = dot(clr_dir, white_dir);
//     if( abs(dotP) > 0.9 ){
//         return abs(dotP);
//     }else{ 
//         return 0.0; 
//     }
// }

void main()
{
    gl_Position = u_model * vec4(position, 1.0);
    f_tCoords = t_coords;
    // vec3 opacity_clr = texture(opacity_texture, f_tCoords).rgb;
    // f_opacity = threshold(opacity_clr);
} 