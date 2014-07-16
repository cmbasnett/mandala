{
	"type":"fragment",
	"source":"#version 150

uniform sampler2D diffuse_texture;

in vec2 out_texcoord;

out vec4 fragment;

void main() 
{
	fragment = texture2D(diffuse_texture, out_texcoord);
}"
}
