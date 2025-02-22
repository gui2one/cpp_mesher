#version 460 core
precision highp float;

uniform vec3 u_Color;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 ColorID; 
void main(){

    FragColor = vec4(u_Color, 1.0);
    ColorID = vec4(0.0, 0.0, 0.0, 0.0);
}