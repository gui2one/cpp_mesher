#version 460 core
precision highp float;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 ColorID;

uniform vec3 u_color;

void main(){
    FragColor = vec4(u_color, 0.8);
    ColorID   = vec4(0.0, 0.0, 0.0, 0.0);
}