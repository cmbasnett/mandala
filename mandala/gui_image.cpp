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
#include "blur_horizontal_gpu_program.hpp"

namespace mandala
{
    gui_image_t::gui_image_t()
    {
        std::array<index_buffer_type::index_type, 4> indices = {
            0, 1, 2, 3
        };

        _index_buffer = std::make_shared<index_buffer_type>();
		_index_buffer->data(indices, gpu_t::buffer_usage_e::static_draw);

        _vertex_buffer = std::make_shared<vertex_buffer_type>();
	}

	void gui_image_t::render(mat4_t world_matrix, mat4_t view_projection_matrix)
    {
        if (is_hidden())
        {
            return;
        }

		auto blend_state = gpu.blend.top();
		blend_state.is_enabled = true;
		blend_state.src_factor = gpu_t::blend_factor_e::src_alpha;
		blend_state.dst_factor = gpu_t::blend_factor_e::one_minus_src_alpha;

		gpu.blend.push(blend_state);

		//buffers
        gpu.buffers.push(gpu_t::buffer_target_e::array, _vertex_buffer);
		gpu.buffers.push(gpu_t::buffer_target_e::element_array, _index_buffer);

		const auto& gpu_program = gui_gpu_program;

		//program
		gpu.programs.push(gpu_program);

		const auto center = bounds().center();

		world_matrix *= glm::translate(center.x, center.y, 0.0f);

		static const auto diffuse_texture_index = 0;

		gpu_program->diffuse_texture_index(diffuse_texture_index);
		gpu_program->world_matrix(world_matrix);
		gpu_program->view_projection_matrix(view_projection_matrix);
		//gpu_program->blur_size(1.0f / 256.0f);

        gpu.textures.bind(diffuse_texture_index, _sprite.sprite_set->texture);

        gpu.draw_elements(gpu_t::primitive_type_e::triangle_fan, 4, gpu_t::index_type_e::unsigned_byte, 0);

        gpu.textures.unbind(diffuse_texture_index);

		gpu.programs.pop();

		gpu.buffers.pop(gpu_t::buffer_target_e::element_array);
		gpu.buffers.pop(gpu_t::buffer_target_e::array);

		gpu.blend.pop();

        gui_node_t::render(world_matrix, view_projection_matrix);
	}

    void gui_image_t::clean()
    {
        const auto sprite_size = static_cast<vec2_t>(_sprite.region.rectangle.size());

		if (_is_autosized_to_texture)
		{
			set_size(static_cast<vec2_t>(_sprite.region.source_size));
        }

		if (_sprite.region.is_rotated)
        {
            vertex_buffer_type::vertex_type vertices[vertex_count] = {
				vertex_type(vec3_t(vec2_t(-0.5f, -0.5f) * sprite_size, 0.0f), vec2_t(_sprite.region.uv.min.x, _sprite.region.uv.max.y)),
				vertex_type(vec3_t(vec2_t( 0.5f, -0.5f) * sprite_size, 0.0f), vec2_t(_sprite.region.uv.min.x, _sprite.region.uv.min.y)),
				vertex_type(vec3_t(vec2_t( 0.5f,  0.5f) * sprite_size, 0.0f), vec2_t(_sprite.region.uv.max.x, _sprite.region.uv.min.y)),
				vertex_type(vec3_t(vec2_t(-0.5f,  0.5f) * sprite_size, 0.0f), vec2_t(_sprite.region.uv.max.x, _sprite.region.uv.max.y))
            };

            _vertex_buffer->data(vertices, vertex_count, gpu_t::buffer_usage_e::dynamic_draw);
        }
        else
        {
            vertex_buffer_type::vertex_type vertices[vertex_count] = {
				vertex_type(vec3_t(vec2_t(-0.5f, -0.5f) * sprite_size, 0.0f), vec2_t(_sprite.region.uv.min.x, _sprite.region.uv.min.y)),
				vertex_type(vec3_t(vec2_t(0.5f, -0.5f) * sprite_size, 0.0f), vec2_t(_sprite.region.uv.max.x, _sprite.region.uv.min.y)),
				vertex_type(vec3_t(vec2_t(0.5f, 0.5f) * sprite_size, 0.0f), vec2_t(_sprite.region.uv.max.x, _sprite.region.uv.max.y)),
				vertex_type(vec3_t(vec2_t(-0.5f, 0.5f) * sprite_size, 0.0f), vec2_t(_sprite.region.uv.min.x, _sprite.region.uv.max.y))
            };

			_vertex_buffer->data(vertices, vertex_count, gpu_t::buffer_usage_e::dynamic_draw);
        }
		
		gui_node_t::clean();
	}
}
