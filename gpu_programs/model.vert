#version 400

uniform mat4 world_matrix;
uniform mat3 normal_matrix;
uniform mat4 view_projection_matrix;
uniform mat4 bone_matrices[128];
uniform vec3 light_location;
uniform vec3 camera_location;

in vec3 location;
in vec3 normal;
in vec3 tangent;
in vec2 texcoord;
in ivec4 bone_indices_0;
in ivec4 bone_indices_1;
in vec4 bone_weights_0;
in vec4 bone_weights_1;

out vec2 out_texcoord;
out vec3 out_location;
out vec3 out_light_vector;
out vec3 out_normal;
out vec3 out_view_direction;

mat4 create_bone_transform()
{
    mat4 bone_transform = mat4(0.0);
    
    bone_transform += (bone_matrices[bone_indices_0[0]] * bone_weights_0[0]);
    bone_transform += (bone_matrices[bone_indices_0[1]] * bone_weights_0[1]);
    bone_transform += (bone_matrices[bone_indices_0[2]] * bone_weights_0[2]);
    bone_transform += (bone_matrices[bone_indices_0[3]] * bone_weights_0[3]);
    bone_transform += (bone_matrices[bone_indices_1[0]] * bone_weights_1[0]);
    bone_transform += (bone_matrices[bone_indices_1[1]] * bone_weights_1[1]);
    bone_transform += (bone_matrices[bone_indices_1[2]] * bone_weights_1[2]);
    bone_transform += (bone_matrices[bone_indices_1[3]] * bone_weights_1[3]);
    
    return bone_transform;
}

void main()
{
    out_texcoord = texcoord;

    mat4 bone_transform = create_bone_transform();
    
    vec4 world_location = world_matrix * bone_transform * vec4(location, 1.0);
    
    out_location = world_location.xyz;
    
    out_normal = normal;
    
    vec3 world_normal = normal_matrix * mat3(bone_transform) * normal;
    vec3 world_tangent = normal_matrix * mat3(bone_transform) * tangent;
    vec3 world_binormal = cross(world_normal, world_tangent);
    
    mat3 tbn_matrix = mat3(world_tangent, world_binormal, world_normal);
    
    out_light_vector = (world_location.xyz - light_location) * tbn_matrix;
    
    vec4 final_location = view_projection_matrix * world_location;
    
    out_view_direction = normalize(camera_location - out_location);
    
    gl_Position = final_location;    
}