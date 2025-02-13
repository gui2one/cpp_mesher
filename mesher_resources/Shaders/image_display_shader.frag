#version 300 es
precision highp float;

uniform sampler2D sampler;
in vec2 vTextureCoord;
out vec4 FragColor;
void main() 
{
    vec4 texture_color = texture(sampler, vTextureCoord);
    FragColor = vec4(texture_color.r, texture_color.g, texture_color.b, 1.0);
}