#version 400

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;

in vec3 location;

out vec4 out_color;

void main() 
{
    gl_Position = view_projection_matrix * (world_matrix * vec4(location, 1));

    out_color = color;
}