#version 300 es
precision highp float;

in vec2 f_t_coords;

uniform sampler2D font_atlas;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 ColorID; 

void main(){

    vec4 color = texture(font_atlas, f_t_coords);

    // if( color.r < 0.5) color.r = 0.0;
    FragColor = vec4(1.0, 1.0, 1.0, color.r);
    ColorID = vec4(0.0, 0.0, 0.0, 1.0);
}