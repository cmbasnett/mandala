{
	"type":"fragment",
	"source":"#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

const float blurSize = 1.0/256.0;

uniform sampler2D color_texture;
uniform vec2 color_texture_size;

in vec2 out_texcoord;

void main()
{
   vec4 sum = vec4(0.0);
   sum += texture2D(color_texture, vec2(out_texcoord.x, out_texcoord.y - 4.0*blurSize)) * 0.05;
   sum += texture2D(color_texture, vec2(out_texcoord.x, out_texcoord.y - 3.0*blurSize)) * 0.09;
   sum += texture2D(color_texture, vec2(out_texcoord.x, out_texcoord.y - 2.0*blurSize)) * 0.12;
   sum += texture2D(color_texture, vec2(out_texcoord.x, out_texcoord.y - blurSize)) * 0.15;
   sum += texture2D(color_texture, vec2(out_texcoord.x, out_texcoord.y)) * 0.16;
   sum += texture2D(color_texture, vec2(out_texcoord.x, out_texcoord.y + blurSize)) * 0.15;
   sum += texture2D(color_texture, vec2(out_texcoord.x, out_texcoord.y + 2.0*blurSize)) * 0.12;
   sum += texture2D(color_texture, vec2(out_texcoord.x, out_texcoord.y + 3.0*blurSize)) * 0.09;
   sum += texture2D(color_texture, vec2(out_texcoord.x, out_texcoord.y + 4.0*blurSize)) * 0.05;
   gl_FragColor = sum;
}"
}
