#version 300 es
precision mediump float;


in vec3 WorldPos;

uniform samplerCube skybox;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 ColorID; 

void main()
{    
    FragColor = texture(skybox, WorldPos);
    ColorID = vec4(0,0,0,1.0);
}