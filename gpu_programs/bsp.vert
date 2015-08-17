#version 400

precision lowp float;

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;
uniform sampler2D diffuse_texture;
uniform sampler2D lightmap_texture;

in vec3 location;
in vec2 diffuse_texcoord;
in vec2 lightmap_texcoord;

out vec2 out_diffuse_texcoord;
out vec2 out_lightmap_texcoord;

void main() 
{
    gl_Position = view_projection_matrix * (world_matrix * vec4(location, 1));
    
    //out_normal = normal;
    out_diffuse_texcoord = diffuse_texcoord;
    out_lightmap_texcoord = lightmap_texcoord;
}