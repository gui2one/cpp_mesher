#version 300 es
precision highp float;

layout ( location = 0) in vec3 position;
layout ( location = 1) in vec3 previous_position;
layout ( location = 2) in vec3 next_position;
layout ( location = 3) in vec3 normal;
layout ( location = 4) in vec2 t_coords;
layout ( location = 5) in float direction;

uniform float u_aspect;

uniform mat4 u_model;
uniform mat4 u_projection;
uniform mat4 u_view;
uniform vec3 u_camera_pos;
uniform float u_dimension;
uniform float u_line_width;

void main(){
    float dist = distance(inverse(u_model) * vec4(u_camera_pos, 1.0), vec4(position, 1.0));
    float thickness = u_line_width / u_dimension;

    mat4 projViewModel = u_projection * u_view * u_model;

    //into clip space
    vec4 currentProjected = projViewModel * vec4(position, 1.0);
    //into NDC space [-1 .. 1]
    vec2 currentScreen = currentProjected.xy / currentProjected.w;
    //correct for aspect ratio (screenWidth / screenHeight)
    currentScreen.x *= u_aspect;
    
    //into clip space
    vec4 previousProjected = projViewModel * vec4(previous_position, 1.0);
    //into NDC space [-1 .. 1]
    vec2 previousScreen = previousProjected.xy / previousProjected.w;
    //correct for aspect ratio (screenWidth / screenHeight)
    previousScreen.x *= u_aspect;

    //into clip space
    vec4 nextProjected = projViewModel * vec4(next_position, 1.0);
    //into NDC space [-1 .. 1]
    vec2 nextScreen = nextProjected.xy / nextProjected.w;
    //correct for aspect ratio (screenWidth / screenHeight)
    nextScreen.x *= u_aspect;


    //normal of line (B - A)
    vec2 dir_prev = normalize(previousScreen - currentScreen);
    vec2 dir_next = normalize(nextScreen - currentScreen);
    vec2 dir = normalize((dir_next + (dir_prev * -1.0)) / 2.0);
    vec2 normal = vec2(-dir.y, dir.x);

    //extrude from center & correct aspect ratio
    normal *= thickness/2.0 * dist;
    normal.x /= u_aspect;

    //offset by the direction of this point in the pair (-1 or 1)
    vec4 offset = vec4(normal * -direction, 0.0, 0.0); // reverse direction because of face culling
    gl_Position = currentProjected + offset;

}