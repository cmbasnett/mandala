{
	"type":"fragment",
	"source":"#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

struct diffuse_t
{
	sampler2D texture;
	vec4 color;
};

uniform diffuse_t diffuse;

in vec2 out_texcoord;

void main()
{
	gl_FragColor = texture2D(diffuse.texture, out_texcoord) * diffuse.color;
}"
}
