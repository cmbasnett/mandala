//glm
#include "glm\ext.hpp"

//mandala
#include "app.hpp"
#include "gpu_program.hpp"
#include "texture.hpp"
#include "sprite_set.hpp"
#include "sprite.hpp"
#include "gui_image.hpp"
#include "gpu.hpp"
#include "gui_gpu_program.hpp"

namespace mandala
{
    gui_image_t::gui_image_t()
    {
        std::array<index_buffer_type::index_type, 4> indices = {
            0, 1, 2, 3
        };

        index_buffer = std::make_shared<index_buffer_type>();
        index_buffer->data(indices, gpu_t::buffer_usage_e::static_draw);

        vertex_buffer = std::make_shared<vertex_buffer_type>();
    }

	void gui_image_t::render(mat4_t world_matrix, mat4_t view_projection_matrix)
    {
        if (is_hidden)
        {
            return;
        }

		gpu_t::blend_t::state_t gpu_blend_state;
		gpu_blend_state.is_enabled = true;
		gpu_blend_state.src_factor = gpu_t::blend_factor_e::src_alpha;
		gpu_blend_state.dst_factor = gpu_t::blend_factor_e::one_minus_src_alpha;

		gpu.blend.push(gpu_blend_state);

        //glDisable(GL_CULL_FACE);

		gpu.programs.push(gui_gpu_program);

        gpu.buffers.push(gpu_t::buffer_target_e::array, vertex_buffer);
        gpu.buffers.push(gpu_t::buffer_target_e::element_array, index_buffer);

        static const auto diffuse_texture_index = 0;

        const auto center = bounds.center();

        world_matrix *= glm::translate(center.x, center.y, 0.0f);

		gui_gpu_program->diffuse_texture_index(diffuse_texture_index);
		gui_gpu_program->world_matrix(world_matrix);
		gui_gpu_program->view_projection_matrix(view_projection_matrix);

        gpu.textures.bind(diffuse_texture_index, sprite.sprite_set->texture);

        gpu.draw_elements(gpu_t::primitive_type_e::triangle_fan, 4, gpu_t::index_type_e::unsigned_byte, 0);

        gpu.textures.unbind(diffuse_texture_index);

        gpu.buffers.pop(gpu_t::buffer_target_e::element_array);
        gpu.buffers.pop(gpu_t::buffer_target_e::array);

        gpu.programs.pop();

		gpu.blend.pop();

        gui_node_t::render(world_matrix, view_projection_matrix);
	}

    bool gui_image_t::clean()
    {
        auto sprite_size = static_cast<vec2_t>(sprite.region.rectangle.size());

		if (is_autosized_to_texture)
		{
			size = static_cast<vec2_t>(sprite.region.source_size);
        }

        if (sprite.region.is_rotated)
        {
            vertex_buffer_type::vertex_type vertices[vertex_count] = {
                vertex_type(vec2_t(0, 0) * sprite_size, vec2_t(sprite.region.uv.min.x, sprite.region.uv.max.y)),
                vertex_type(vec2_t(1, 0) * sprite_size, vec2_t(sprite.region.uv.min.x, sprite.region.uv.min.y)),
                vertex_type(vec2_t(1, 1) * sprite_size, vec2_t(sprite.region.uv.max.x, sprite.region.uv.min.y)),
                vertex_type(vec2_t(0, 1) * sprite_size, vec2_t(sprite.region.uv.max.x, sprite.region.uv.max.y))
            };

            vertex_buffer->data(vertices, vertex_count, gpu_t::buffer_usage_e::dynamic_draw);
        }
        else
        {
            vertex_buffer_type::vertex_type vertices[vertex_count] = {
                vertex_type(vec2_t(0, 0) * sprite_size, vec2_t(sprite.region.uv.min.x, sprite.region.uv.min.y)),
                vertex_type(vec2_t(1, 0) * sprite_size, vec2_t(sprite.region.uv.max.x, sprite.region.uv.min.y)),
                vertex_type(vec2_t(1, 1) * sprite_size, vec2_t(sprite.region.uv.max.x, sprite.region.uv.max.y)),
                vertex_type(vec2_t(0, 1) * sprite_size, vec2_t(sprite.region.uv.min.x, sprite.region.uv.max.y))
            };

            vertex_buffer->data(vertices, vertex_count, gpu_t::buffer_usage_e::dynamic_draw);
        }

		return gui_node_t::clean();
	}
};
