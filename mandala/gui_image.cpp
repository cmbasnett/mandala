//glm
#include "glm\ext.hpp"

//mandala
#include "gpu_program_mgr.hpp"
#include "gpu_program.hpp"
#include "texture.hpp"
#include "sprite_set.hpp"
#include "sprite.hpp"
#include "gui_image.hpp"
#include "gpu.hpp"
#include "gui_gpu_program.hpp"
#include "blur_horizontal_gpu_program.hpp"
#include "collision.hpp"
#include "input_event.hpp"

namespace mandala
{
    gui_image_t::gui_image_t()
    {
		//index buffer
        std::initializer_list<index_buffer_type::index_type> indices = {
            0, 1, 2, 
            0, 2, 3, 
            1, 2, 3, 
            1, 3, 0 };

        _index_buffer = std::make_shared<index_buffer_type>();
		_index_buffer->data(indices, gpu_t::buffer_usage_e::static_draw);

		//vertex buffer
        _vertex_buffer = std::make_shared<vertex_buffer_type>();
	}

	void gui_image_t::render_override(mat4_t world_matrix, mat4_t view_projection_matrix)
    {
		auto blend_state = gpu.blend.get_state();
		blend_state.is_enabled = true;
		blend_state.src_factor = gpu_t::blend_factor_e::src_alpha;
		blend_state.dst_factor = gpu_t::blend_factor_e::one_minus_src_alpha;

        gpu.blend.push_state(blend_state);

        //buffers
        gpu.buffers.push(gpu_t::buffer_target_e::array, _vertex_buffer);
        gpu.buffers.push(gpu_t::buffer_target_e::element_array, _index_buffer);

        const auto& gpu_program = gpu_programs.get<gui_gpu_program_t>();

		//program
		gpu.programs.push(gpu_program);

		const auto center = bounds().center();

		world_matrix *= glm::translate(center.x, center.y, 0.0f);

		static const auto diffuse_texture_index = 0;

		gpu_program->diffuse_texture_index(diffuse_texture_index);
		gpu_program->world_matrix(world_matrix);
		gpu_program->view_projection_matrix(view_projection_matrix);
		gpu_program->color(color());

        gpu.textures.bind(diffuse_texture_index, _sprite.sprite_set->texture);

        switch (_triangle_mode)
        {
        case triangle_mode_e::bottom_right:
            gpu.draw_elements(gpu_t::primitive_type_e::triangles, 3, gpu_t::index_type_e::unsigned_byte, 0);
            break;
        case triangle_mode_e::top_left:
            gpu.draw_elements(gpu_t::primitive_type_e::triangles, 3, gpu_t::index_type_e::unsigned_byte, 3);
            break;
        case triangle_mode_e::top_right:
            gpu.draw_elements(gpu_t::primitive_type_e::triangles, 3, gpu_t::index_type_e::unsigned_byte, 6);
            break;
        case triangle_mode_e::bottom_left:
            gpu.draw_elements(gpu_t::primitive_type_e::triangles, 3, gpu_t::index_type_e::unsigned_byte, 9);
            break;
        case triangle_mode_e::both:
            gpu.draw_elements(gpu_t::primitive_type_e::triangles, 6, gpu_t::index_type_e::unsigned_byte, 0);
            break;
        }

        gpu.textures.unbind(diffuse_texture_index);

		gpu.programs.pop();

		gpu.buffers.pop(gpu_t::buffer_target_e::element_array);
		gpu.buffers.pop(gpu_t::buffer_target_e::array);

		gpu.blend.pop_state();

        gui_node_t::render_override(world_matrix, view_projection_matrix);
	}

    void gui_image_t::clean()
    {
        auto sprite_size = static_cast<vec2_t>(_sprite.region.rectangle.size());

		if (_is_autosized_to_texture)
		{
			set_size(static_cast<vec2_t>(_sprite.region.source_size));
        }
		else
		{
			set_size(bounds().size());

			sprite_size = bounds().size();
		}

        std::array<vertex_type::position_type, vertex_count> vertex_positions = {
            vertex_type::position_type(vec3_t(vec2_t(-0.5f, -0.5f) * sprite_size, 0.0f)),
            vertex_type::position_type(vec3_t(vec2_t(0.5f, -0.5f) * sprite_size, 0.0f)),
            vertex_type::position_type(vec3_t(vec2_t(0.5f, 0.5f) * sprite_size, 0.0f)),
            vertex_type::position_type(vec3_t(vec2_t(-0.5f, 0.5f) * sprite_size, 0.0f))
        };

		if (_sprite.region.is_rotated)
        {
            vertex_type vertices[vertex_count] = {
                vertex_type(vertex_positions[0], vec2_t(_sprite.region.uv.min.x, _sprite.region.uv.max.y)),
                vertex_type(vertex_positions[1], vec2_t(_sprite.region.uv.min.x, _sprite.region.uv.min.y)),
                vertex_type(vertex_positions[2], vec2_t(_sprite.region.uv.max.x, _sprite.region.uv.min.y)),
                vertex_type(vertex_positions[3], vec2_t(_sprite.region.uv.max.x, _sprite.region.uv.max.y))
            };

            _vertex_buffer->data(vertices, vertex_count, gpu_t::buffer_usage_e::dynamic_draw);
        }
        else
        {
            vertex_type vertices[vertex_count] = {
                vertex_type(vertex_positions[0], vec2_t(_sprite.region.uv.min.x, _sprite.region.uv.min.y)),
                vertex_type(vertex_positions[1], vec2_t(_sprite.region.uv.max.x, _sprite.region.uv.min.y)),
                vertex_type(vertex_positions[2], vec2_t(_sprite.region.uv.max.x, _sprite.region.uv.max.y)),
                vertex_type(vertex_positions[3], vec2_t(_sprite.region.uv.min.x, _sprite.region.uv.max.y))
            };

			_vertex_buffer->data(vertices, vertex_count, gpu_t::buffer_usage_e::dynamic_draw);
        }
		
		gui_node_t::clean();
    }

    void gui_image_t::on_input_event(input_event_t& input_event)
    {
        if (input_event.device_type == input_event_t::device_type_e::touch &&
            input_event.touch.type == input_event_t::touch_t::type_e::press)
        {
            if (contains(bounds(), input_event.touch.position))
            {
            }
        }

        gui_node_t::on_input_event(input_event);
    }
}
