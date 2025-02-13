#version 460 core
precision highp float;

uniform sampler2D sampler;
in vec2 vTextureCoord;
out vec4 FragColor;
void main() 
{
    vec4 texture_color = texture(sampler, vec2(vTextureCoord.x, vTextureCoord.y));
    float exponent = 1.0 / 2.2;
    // texture_color.x = pow(texture_color.x, exponent);
    // texture_color.y = pow(texture_color.y, exponent);
    // texture_color.z = pow(texture_color.z, exponent);
    FragColor = texture_color; // * vec4(0.0, 0.5, 0.0, 1.0);
    // FragColor = vec4(0.0, 0.5, 0.0, 1.0);
}