{
	"type":"vertex",
	"source":"#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

uniform mat4 world_matrix;
uniform mat3 normal_matrix;
uniform mat4 view_projection_matrix;
uniform mat4 bone_matrices[128];

in vec3 position;
in vec2 texcoord;
in ivec4 bone_indices_0;
in ivec4 bone_indices_1;
in vec4 bone_weights_0;
in vec4 bone_weights_1;

out vec2 out_texcoord;

mat4 create_bone_transform()
{
	mat4 bone_transform;
	
	bone_transform += bone_matrices[bone_indices_0[0]] * bone_weights_0[0];
	bone_transform += bone_matrices[bone_indices_0[1]] * bone_weights_0[1];
	bone_transform += bone_matrices[bone_indices_0[2]] * bone_weights_0[2];
	bone_transform += bone_matrices[bone_indices_0[3]] * bone_weights_0[3];
	bone_transform += bone_matrices[bone_indices_1[0]] * bone_weights_1[0];
	bone_transform += bone_matrices[bone_indices_1[1]] * bone_weights_1[1];
	bone_transform += bone_matrices[bone_indices_1[2]] * bone_weights_1[2];
	bone_transform += bone_matrices[bone_indices_1[3]] * bone_weights_1[3];
	
	return bone_transform;
}

void main() 
{
	out_texcoord = texcoord;
	
	mat4 bone_transform = create_bone_transform();
	
	vec4 world_position = world_matrix * bone_transform * vec4(position, 1.0);
	
	gl_Position = view_projection_matrix * world_position;
}"
}
