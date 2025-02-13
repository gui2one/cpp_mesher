#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

layout(location=3) in vec2 t_coords[];

uniform mat4 shadowMatrices[6];

uniform sampler2D opacity_texture;

out vec4 FragPos; // FragPos from GS (output per emitvertex)

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
    vec3 opacity_clr = texture(opacity_texture, t_coords[0]).rgb;
    float opacity = threshold(opacity_clr);    
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle vertex
        {

            FragPos = gl_in[i].gl_Position;
            gl_Position = (shadowMatrices[face] * (FragPos)) ;
            EmitVertex();
        }    
        EndPrimitive();
    }
}  