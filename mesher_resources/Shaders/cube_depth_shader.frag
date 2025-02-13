#version 460 core
precision highp float;

in vec4 FragPos;


uniform vec3 u_lightPos;
uniform float u_farPlane;


void main()
{        
        
    // get distance between fragment and light source
    float lightDistance = length(FragPos.xyz - u_lightPos) ;
    
    // map to [0;1] range by dividing by farPlane
    lightDistance = lightDistance / (u_farPlane);
    
    // write this as modified depth
    gl_FragDepth = lightDistance ;
}  