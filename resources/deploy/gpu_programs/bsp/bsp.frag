{
	"type":"fragment",
	"source":"#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

uniform sampler2D diffuse_texture;
uniform sampler2D lightmap_texture;
uniform float lightmap_gamma;

in vec3 out_normal;
in vec2 out_diffuse_texcoord;
in vec2 out_lightmap_texcoord;

void main() 
{
	vec4 lightmap_term = texture(lightmap_texture, out_lightmap_texcoord);
	lightmap_term.xyz *= lightmap_gamma;
	gl_FragColor = texture(diffuse_texture, out_diffuse_texcoord) * lightmap_term;
}"
}
