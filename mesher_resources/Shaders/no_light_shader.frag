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
    vec3 view_dir = normalize(f_FragPos - u_viewPos);
    float dotP = dot(f_Normal, view_dir);
    return vec3(pow(-dotP, 1.5));  
}
void main() {


    FragColor = vec4(compute_color(), 1.0);
    ColorID = vec4(u_ColorID, 1.0);
}