#version 300 es
precision highp float;


uniform vec3 u_color;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 ColorID; 

in vec3 f_Color;
void main(){

    FragColor = vec4(f_Color, 1.0);
    ColorID = vec4(0.0, 0.0, 0.0, 0.0);
}