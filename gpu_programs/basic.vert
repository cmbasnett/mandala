#version 400

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;
uniform float texture_scale;

in vec3 location;
in vec4 color;

out vec4 out_color;
out vec2 out_texcoord;

void main()
{
    gl_Position = view_projection_matrix * (world_matrix * vec4(location, 1));

    out_texcoord.x = location.x * texture_scale;
    out_texcoord.y = location.z * texture_scale;

    out_color = color;
}
