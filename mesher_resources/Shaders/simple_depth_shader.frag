#version 460 core
precision highp float;

in vec2 f_tCoords;
uniform sampler2D opacity_texture;

out vec4 FragColor;

float threshold(vec3 clr)
{
    vec3 clr_dir = normalize(clr);
    vec3 white_dir = normalize(vec3(1.0, 1.0, 1.0));
    float dotP = dot(clr_dir, white_dir);
    if( abs(dotP) > 0.9 ){
        return abs(dotP);
    }else{ 
        return 0.0; 
    }
}
void main()
{             
    vec3 opacity_clr = texture(opacity_texture, f_tCoords).rgb;

    float opacity = threshold(opacity_clr);
    gl_FragDepth = gl_FragCoord.z + (1.0-opacity);
    FragColor = vec4(vec3(gl_FragCoord.z),1.0);
}  