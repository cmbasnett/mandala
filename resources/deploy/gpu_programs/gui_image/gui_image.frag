{
	"type":"fragment",
	"source":"#version 150

uniform sampler2D diffuse_texture;

in vec2 out_texcoord;

void main() 
{
	gl_FragColor = texture2D(diffuse_texture, out_texcoord);
}"
}
