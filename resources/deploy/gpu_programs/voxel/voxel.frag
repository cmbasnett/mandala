{
	"type":"fragment",
	"source":"#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

in vec4 out_color;

void main() 
{
	gl_FragColor = out_color;
}"
}
