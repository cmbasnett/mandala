//glm
#include "glm\ext.hpp"

//mandala
#include "mandala.hpp"
#include "app.hpp"
#include "gpu_program.hpp"
#include "texture.hpp"
#include "sprite_set.hpp"
#include "sprite.hpp"
#include "gui_image.hpp"
#include "gpu_mgr.hpp"

namespace mandala
{
    gui_image_t::gui_image_t()
    {
        std::array<index_buffer_type::index_type, 4> indices = {
            0, 1, 2, 3
        };

        index_buffer = std::make_shared<index_buffer_type>();
        index_buffer->data(indices, gpu_mgr_t::buffer_usage_e::stream_draw);

        vertex_buffer = std::make_shared<vertex_buffer_type>();
    }

	void gui_image_t::render(mat4_t world_matrix, mat4_t view_projection_matrix)
	{
		if (is_hidden)
		{
			return;
		}

		//push GL states
		GLint blend_src_rgb;
        GLint blend_dst_alpha;
        bool is_blend_enabled = glIsEnabled(GL_BLEND) != 0; glCheckError();

        glGetIntegerv(GL_BLEND_SRC_RGB, &blend_src_rgb); glCheckError();
        glGetIntegerv(GL_BLEND_SRC_ALPHA, &blend_dst_alpha); glCheckError();

        glEnable(GL_BLEND); glCheckError();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glCheckError();

        auto gpu_program = app.resources.get<gpu_program_t>(hash_t("gui_image.gpu"));

        gpu.programs.push(gpu_program);

        auto diffuse_texture_location = glGetUniformLocation(gpu_program->id, "diffuse_texture"); glCheckError();
        auto view_projection_matrix_location = glGetUniformLocation(gpu_program->id, "view_projection_matrix"); glCheckError();
        auto world_matrix_location = glGetUniformLocation(gpu_program->id, "world_matrix");  glCheckError();
        auto position_location = glGetAttribLocation(gpu_program->id, "position"); glCheckError();
        auto texcoord_location = glGetAttribLocation(gpu_program->id, "texcoord"); glCheckError();
        static const auto* position_offset = reinterpret_cast<void*>(offsetof(vertex_buffer_type::vertex_type, position));
        static const auto* texcoord_offset = reinterpret_cast<void*>(offsetof(vertex_buffer_type::vertex_type, texcoord));

        static const auto diffuse_texture_index = 0;

        gpu.buffers.push(gpu_mgr_t::buffer_target_e::array, vertex_buffer);
        gpu.buffers.push(gpu_mgr_t::buffer_target_e::element_array, index_buffer);

        const auto center = bounds.center();

        world_matrix *= glm::translate(center.x, center.y, 0.0f);
        //world_matrix *= glm::scale(size.x, size.y, 0.0f);

        glUniform1i(diffuse_texture_location, diffuse_texture_index); glCheckError();
        glUniformMatrix4fv(view_projection_matrix_location, 1, GL_FALSE, glm::value_ptr(view_projection_matrix)); glCheckError();
        glUniformMatrix4fv(world_matrix_location, 1, GL_FALSE, glm::value_ptr(world_matrix)); glCheckError();

        glEnableVertexAttribArray(position_location); glCheckError();
        glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, vertex_buffer_type::vertex_size, position_offset); glCheckError();

        glEnableVertexAttribArray(texcoord_location); glCheckError();
        glVertexAttribPointer(texcoord_location, 2, GL_FLOAT, GL_FALSE, vertex_buffer_type::vertex_size, texcoord_offset); glCheckError();

        gpu.textures.bind(diffuse_texture_index, sprite.sprite_set->texture);

        glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, nullptr); glCheckError();

        gpu.textures.unbind(diffuse_texture_index);

        glDisableVertexAttribArray(texcoord_location); glCheckError();
        glDisableVertexAttribArray(position_location); glCheckError();

        gpu.buffers.pop(gpu_mgr_t::buffer_target_e::element_array);
        gpu.buffers.pop(gpu_mgr_t::buffer_target_e::array);

        gpu.programs.pop();

        //blend
        if (is_blend_enabled)
        {
            glEnable(GL_BLEND);
        }
        else
        {
            glDisable(GL_BLEND);
        }

		//blend function
		glBlendFunc(blend_src_rgb, blend_dst_alpha);

        gui_node_t::render(world_matrix, view_projection_matrix);
	}

    bool gui_image_t::clean()
	{
		if (is_autosized_to_texture)
		{
			size = static_cast<vec2_t>(sprite.region.source_size);
        }

        if (sprite.region.is_rotated)
        {
            vertex_buffer_type::vertex_type vertices[vertex_count] = {
                vertex_type(vec2_t(-0.5f, -0.5f) * size, vec2_t(sprite.region.uv.min.x, sprite.region.uv.max.y)),
                vertex_type(vec2_t(0.5f, -0.5f) * size, vec2_t(sprite.region.uv.min.x, sprite.region.uv.min.y)),
                vertex_type(vec2_t(0.5f, 0.5f) * size, vec2_t(sprite.region.uv.max.x, sprite.region.uv.min.y)),
                vertex_type(vec2_t(-0.5f, 0.5f) * size, vec2_t(sprite.region.uv.max.x, sprite.region.uv.max.y))
            };

            vertex_buffer->data(vertices, vertex_count, gpu_mgr_t::buffer_usage_e::dynamic_draw);
        }
        else
        {
            vertex_buffer_type::vertex_type vertices[vertex_count] = {
                vertex_type(vec2_t(-0.5f, -0.5f) * size, vec2_t(sprite.region.uv.min.x, sprite.region.uv.min.y)),
                vertex_type(vec2_t(0.5f, -0.5f) * size, vec2_t(sprite.region.uv.max.x, sprite.region.uv.min.y)),
                vertex_type(vec2_t(0.5f, 0.5f) * size, vec2_t(sprite.region.uv.max.x, sprite.region.uv.max.y)),
                vertex_type(vec2_t(-0.5f, 0.5f) * size, vec2_t(sprite.region.uv.min.x, sprite.region.uv.max.y))
            };

            vertex_buffer->data(vertices, vertex_count, gpu_mgr_t::buffer_usage_e::dynamic_draw);
        }

		return gui_node_t::clean();
	}
};
