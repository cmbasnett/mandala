//mandala
#include "shadow_gpu_program.hpp"
#include "opengl.hpp"

//glm
#include <glm\ext.hpp>

namespace mandala
{
    std::string shadow_gpu_program_t::vertex_shader_source = R"(
#version 400

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;

in vec3 position;
in vec4 color;

out vec4 out_color;

void main() 
{
	gl_Position = view_projection_matrix * (world_matrix * vec4(position, 1));

	out_color = color;
})";

    std::string shadow_gpu_program_t::fragment_shader_source = R"(
#version 400

in vec4 out_color;

out vec4 fragment;

void main() 
{
	fragment = out_color;
})";

    shadow_gpu_program_t::shadow_gpu_program_t() :
        gpu_program_t(vertex_shader_source, fragment_shader_source)
    {
        position_location = glGetAttribLocation(id, "position"); glCheckError();

        world_matrix_location = glGetUniformLocation(id, "world_matrix"); glCheckError();
        view_projection_matrix_location = glGetUniformLocation(id, "view_projection_matrix"); glCheckError();
    }

    void shadow_gpu_program_t::on_bind()
    {
        static const auto position_offset = reinterpret_cast<void*>(offsetof(vertex_type, position));

        glEnableVertexAttribArray(position_location); glCheckError();

        glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_type), position_offset); glCheckError();
    }

    void shadow_gpu_program_t::on_unbind()
    {
        glDisableVertexAttribArray(position_location); glCheckError();
    }

    void shadow_gpu_program_t::world_matrix(const mat4_t& world_matrix) const
    {
        glUniformMatrix4fv(world_matrix_location, 1, GL_FALSE, glm::value_ptr(world_matrix)); glCheckError();
    }

    void shadow_gpu_program_t::view_projection_matrix(const mat4_t& view_projection_matrix) const
    {
        glUniformMatrix4fv(view_projection_matrix_location, 1, GL_FALSE, glm::value_ptr(view_projection_matrix)); glCheckError();
    }
}
