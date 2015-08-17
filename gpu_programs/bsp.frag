#version 400

uniform sampler2D diffuse_texture;
uniform sampler2D lightmap_texture;
uniform float lightmap_gamma;
uniform float alpha;
uniform bool should_test_alpha;

in vec3 out_normal;
in vec2 out_diffuse_texcoord;
in vec2 out_lightmap_texcoord;

out vec4 fragment;

void main() 
{
    vec4 lightmap_term = texture(lightmap_texture, out_lightmap_texcoord);
    lightmap_term.xyz *= lightmap_gamma;
    
    vec4 diffuse_term = texture(diffuse_texture, out_diffuse_texcoord);
    
    fragment = diffuse_term * lightmap_term;
    
    fragment.a *= alpha;
    
    if(should_test_alpha && fragment.a <= 0.25)
    {
        discard;
    }
}