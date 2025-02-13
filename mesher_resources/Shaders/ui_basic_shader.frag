#version 300 es
precision highp float;

in vec2 f_t_coords;
in vec3 f_color;

layout(location = 0)out vec4 FragColor;
layout(location = 1) out vec4 ColorID; 

void main(){

    FragColor = vec4(f_color,1.0);
    ColorID = vec4(0.0, 0.0, 0.0, 1.0);
}