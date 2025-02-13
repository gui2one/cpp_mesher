#version 460 core
in vec2 f_texCoords;
in vec4 f_fragColor;

uniform sampler2D msdf;

uniform vec3 u_viewPos;
uniform vec3 u_ColorID;

uniform float pxRange; // set to distance field's pixel range

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 ColorID;



float screenPxRange() {
    vec2 unitRange = vec2(pxRange)/vec2(textureSize(msdf, 0));
    vec2 screenTexSize = vec2(1.0)/fwidth(f_texCoords);
    return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}
void main()
{

    vec4 fgColor = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 bgColor = vec4(0.0, 0.0, 0.0, 0.0);
    
    vec3 msd = texture(msdf, f_texCoords).rgb;
    float sd = median(msd.r, msd.g, msd.b);


    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    if(opacity < 0.8) discard;
    FragColor = mix(bgColor, f_fragColor, opacity);

    ColorID = vec4(u_ColorID, 1.0);

}