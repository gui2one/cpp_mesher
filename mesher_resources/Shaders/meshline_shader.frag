#version 300 es
precision highp float;

uniform vec3 u_ColorID;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 ColorID; 
void main(){

    FragColor = vec4(1.0, 0.2, 0.2, 1.0);
    ColorID = vec4(u_ColorID, 1.0);
}