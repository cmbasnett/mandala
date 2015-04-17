//glm
#include "glm\ext.hpp"

//mandala
#include "gpu_program_mgr.hpp"
#include "gpu_buffer_mgr.hpp"
#include "gpu_program.hpp"
#include "texture.hpp"
#include "sprite_set.hpp"
#include "sprite.hpp"
#include "gui_image.hpp"
#include "gpu.hpp"
#include "gui_image_gpu_program.hpp"
#include "blur_horizontal_gpu_program.hpp"
#include "collision.hpp"
#include "input_event.hpp"

#include "line_renderer.hpp"

namespace mandala
{
    gui_image_t::gui_image_t() :
        index_buffer(gpu_buffers.make<index_buffer_type>()),
        vertex_buffer(gpu_buffers.make<vertex_buffer_type>())
    {
        std::initializer_list<index_buffer_type::index_type> indices = {
            //non-rotated
            0, 1, 2, 
            0, 2, 3, 
            1, 2, 3, 
            1, 3, 0,
            //rotated
            //4, 5, 6,
            //4, 6, 7,
            //5, 6, 7,
            //5, 7, 4
        };
		index_buffer->data(indices, gpu_t::buffer_usage_e::static_draw);
	}

    void gui_image_t::on_cleaned()
    {
        const auto& sprite_region = sprite->get_region();
        auto scale = static_cast<vec2_t>(sprite_region.frame_rectangle.size()) / static_cast<vec2_t>(sprite_region.source_size);

        if (sprite_region.is_rotated)
        {
            std::swap(scale.x, scale.y);
        }

        auto sprite_size = get_size() * scale;

        std::array<vertex_type::location_type, vertex_count> vertex_locations = {
            vertex_type::location_type(vec3_t(vec2_t(-0.5f, -0.5f) * sprite_size, 0.0f)),
            vertex_type::location_type(vec3_t(vec2_t( 0.5f, -0.5f) * sprite_size, 0.0f)),
            vertex_type::location_type(vec3_t(vec2_t( 0.5f,  0.5f) * sprite_size, 0.0f)),
            vertex_type::location_type(vec3_t(vec2_t(-0.5f,  0.5f) * sprite_size, 0.0f))
        };

        auto vertices = {
            vertex_type(vertex_locations[0], rgba_type(1), vec2_t(0, 0)),
            vertex_type(vertex_locations[1], rgba_type(1), vec2_t(1, 0)),
            vertex_type(vertex_locations[2], rgba_type(1), vec2_t(1, 1)),
            vertex_type(vertex_locations[3], rgba_type(1), vec2_t(0, 1))
            //non-rotated
            //vertex_type(vertex_locations[0], rgba_type(1), vec2_t(sprite_region.uv.min.x, sprite_region.uv.min.y)),
            //vertex_type(vertex_locations[1], rgba_type(1), vec2_t(sprite_region.uv.max.x, sprite_region.uv.min.y)),
            //vertex_type(vertex_locations[2], rgba_type(1), vec2_t(sprite_region.uv.max.x, sprite_region.uv.max.y)),
            //vertex_type(vertex_locations[3], rgba_type(1), vec2_t(sprite_region.uv.min.x, sprite_region.uv.max.y)),
            ////rotated
            //vertex_type(vertex_locations[0], rgba_type(1), vec2_t(sprite_region.uv.min.x, sprite_region.uv.max.y)),
            //vertex_type(vertex_locations[1], rgba_type(1), vec2_t(sprite_region.uv.min.x, sprite_region.uv.min.y)),
            //vertex_type(vertex_locations[2], rgba_type(1), vec2_t(sprite_region.uv.max.x, sprite_region.uv.min.y)),
            //vertex_type(vertex_locations[3], rgba_type(1), vec2_t(sprite_region.uv.max.x, sprite_region.uv.max.y))
        };

        vertex_buffer->data(vertices, gpu_t::buffer_usage_e::static_draw);
    }

    void gui_image_t::set_sprite(boost::optional<sprite_t> sprite)
    {
        this->sprite = sprite;

        if (sprite && is_autosized_to_texture)
        {
            set_size(static_cast<gui_node_t::size_type>(sprite->get_region().source_size));
        }
    }

    void gui_image_t::set_is_autosized_to_texture(bool is_autosized_to_texture)
    {
        this->is_autosized_to_texture = is_autosized_to_texture;

        set_sprite(sprite);
    }

	void gui_image_t::on_render_begin(const mat4_t& world_matrix, const mat4_t& view_projection_matrix)
    {
		auto blend_state = gpu.blend.get_state();
		blend_state.is_enabled = true;
		blend_state.src_factor = gpu_t::blend_factor_e::src_alpha;
		blend_state.dst_factor = gpu_t::blend_factor_e::one_minus_src_alpha;

        gpu.blend.push_state(blend_state);

        //buffers
        gpu.buffers.push(gpu_t::buffer_target_e::array, vertex_buffer);
        gpu.buffers.push(gpu_t::buffer_target_e::element_array, index_buffer);

        const auto& gpu_program = gpu_programs.get<gui_image_gpu_program_t>();

		//program
		gpu.programs.push(gpu_program);

		const auto center = get_bounds().center();

		static const auto diffuse_texture_index = 0;

        gpu.set_uniform("diffuse_texture", diffuse_texture_index);
        gpu.set_uniform("world_matrix", world_matrix * glm::translate(center.x, center.y, 0.0f));
        gpu.set_uniform("view_projection_matrix", view_projection_matrix);
        gpu.set_uniform("color", get_color());
        gpu.set_uniform("region_min", sprite->get_region().uv.min);
        gpu.set_uniform("region_size", sprite->get_region().uv.size());

        gpu.textures.bind(diffuse_texture_index, sprite->get_sprite_set()->get_texture());

        size_t index_offset = 0;

        //if (sprite->get_region().is_rotated)
        //{
        //    index_offset = 12;
        //}

        switch (triangle_mode)
        {
        case triangle_mode_e::bottom_right:
            gpu.draw_elements(gpu_t::primitive_type_e::triangles, 3, index_buffer_type::data_type, index_offset + 0);
            break;
        case triangle_mode_e::top_left:
            gpu.draw_elements(gpu_t::primitive_type_e::triangles, 3, index_buffer_type::data_type, index_offset + 3);
            break;
        case triangle_mode_e::top_right:
            gpu.draw_elements(gpu_t::primitive_type_e::triangles, 3, index_buffer_type::data_type, index_offset + 6);
            break;
        case triangle_mode_e::bottom_left:
            gpu.draw_elements(gpu_t::primitive_type_e::triangles, 3, index_buffer_type::data_type, index_offset + 9);
            break;
        case triangle_mode_e::both:
            gpu.draw_elements(gpu_t::primitive_type_e::triangles, 6, index_buffer_type::data_type, index_offset + 0);
            break;
        }

        gpu.textures.unbind(diffuse_texture_index);

		gpu.programs.pop();

		gpu.buffers.pop(gpu_t::buffer_target_e::element_array);
		gpu.buffers.pop(gpu_t::buffer_target_e::array);

		gpu.blend.pop_state();

        gui_node_t::on_render_begin(world_matrix, view_projection_matrix);
	}

    void gui_image_t::on_input_event(input_event_t& input_event)
    {
        if (input_event.device_type == input_event_t::device_type_e::touch &&
            input_event.touch.type == input_event_t::touch_t::type_e::press)
        {
            if (contains(get_bounds(), input_event.touch.location))
            {
                std::cout << "touched image" << std::endl;
            }
        }

        gui_node_t::on_input_event(input_event);
    }
}
