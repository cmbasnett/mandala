{
	"type":"fragment",
	"source":"#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

uniform sampler2D diffuse_map;

in vec2 out_texcoord;
in vec4 out_color;

void main() 
{
	gl_FragColor = texture2D(diffuse_map, out_texcoord) * out_color;
}"
}
