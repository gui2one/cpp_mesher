#version 300 es
precision highp float;


uniform vec3 u_color;
uniform sampler2D u_Texture;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 ColorID; 

in vec3 f_Color;
in vec2 f_TexCoords;
void main(){

    FragColor = texture(u_Texture, f_TexCoords);
    ColorID = vec4(0.0, 0.0, 0.0, 0.0);
}