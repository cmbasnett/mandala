#version 400

uniform sampler2D diffuse_texture;

in vec2 out_texcoord;
in vec4 out_color;

void main() 
{
    gl_FragColor = texture2D(diffuse_texture, out_texcoord) * out_color;
}