#version 460 core
precision highp float;

/*
    include external files using GLSL-Shader-Includes library ( in vendor/ )
*/
// clang-format off
#include incl/basic_material.glsl

// clang-format on

uniform Material material;


uniform vec3 u_ColorID;

in vec3 f_FragPos;
in vec3 f_Normal;
in vec3 f_Color;
in vec2 f_TexCoords;

uniform vec3 u_viewPos;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 ColorID;

vec3 compute_color(){
    vec3 result = vec3(0.0,0.0,1.0);

    return result;
}
void main() {


    FragColor = vec4(f_Normal, 1.0);
    ColorID = vec4(u_ColorID, 1.0);
}