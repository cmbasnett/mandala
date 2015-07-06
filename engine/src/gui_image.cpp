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
		index_buffer->data(indices, gpu_t::buffer_usage_e::STATIC_DRAW);
	}

    void gui_image_t::on_clean_end()
    {
        const auto& sprite_region = sprite->get_region();
        auto scale = static_cast<vec2_t>(sprite_region.frame_rectangle.size()) / static_cast<vec2_t>(sprite_region.source_size);
        auto center_offset = static_cast<vec2_t>(sprite_region.rectangle.center()) - (static_cast<vec2_t>(sprite_region.source_size) * 0.5f);

        if (sprite_region.is_rotated)
        {
            std::swap(scale.x, scale.y);
            std::swap(center_offset.x, center_offset.y);
        }

        auto sprite_size = get_size() * scale;

        std::array<vertex_type::location_type, VERTEX_COUNT> vertex_locations = {
            vertex_type::location_type(vec3_t(center_offset + (vec2_t(-0.5f, -0.5f) * sprite_size), 0.0f)),
            vertex_type::location_type(vec3_t(center_offset + (vec2_t(0.5f, -0.5f) * sprite_size), 0.0f)),
            vertex_type::location_type(vec3_t(center_offset + (vec2_t(0.5f, 0.5f) * sprite_size), 0.0f)),
            vertex_type::location_type(vec3_t(center_offset + (vec2_t(-0.5f, 0.5f) * sprite_size), 0.0f))
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

        vertex_buffer->data(vertices, gpu_t::buffer_usage_e::STATIC_DRAW);
    }

    void gui_image_t::set_sprite(boost::optional<sprite_t> sprite)
    {
        this->sprite = sprite;

        if (sprite && is_autosized_to_sprite)
        {
            set_size_modes(gui_size_modes_t(gui_size_mode_e::ABSOLUTE, gui_size_mode_e::ABSOLUTE));
            set_size(static_cast<gui_node_t::size_type>(sprite->get_region().source_size));
        }
    }

    void gui_image_t::set_is_autosized_to_sprite(bool is_autosized_to_sprite)
    {
        this->is_autosized_to_sprite = is_autosized_to_sprite;

        set_sprite(sprite);
    }

	void gui_image_t::on_render_begin(mat4_t& world_matrix, mat4_t& view_projection_matrix)
    {
		auto blend_state = gpu.blend.get_state();
		blend_state.is_enabled = true;
		blend_state.src_factor = gpu_t::blend_factor_e::SRC_ALPHA;
		blend_state.dst_factor = gpu_t::blend_factor_e::ONE_MINUS_SRC_ALPHA;

        gpu.blend.push_state(blend_state);

        //buffers
        gpu.buffers.push(gpu_t::buffer_target_e::ARRAY, vertex_buffer);
        gpu.buffers.push(gpu_t::buffer_target_e::ELEMENT_ARRAY, index_buffer);

        const auto& gpu_program = gpu_programs.get<gui_image_gpu_program_t>();

		//program
		gpu.programs.push(gpu_program);

		const auto center = get_bounds().center();

        //TODO: 

		static const auto DIFFUSE_TEXTURE_INDEX = 0;

        gpu.set_uniform("diffuse_texture", DIFFUSE_TEXTURE_INDEX);
        gpu.set_uniform("world_matrix", world_matrix * glm::translate(center.x, center.y, 0.0f));
        gpu.set_uniform("view_projection_matrix", view_projection_matrix);
        gpu.set_uniform("color", get_color());

        //TODO: what to do if sprite is null?
        if (sprite)
        {
            gpu.set_uniform("region_min", sprite->get_region().uv.min);
            gpu.set_uniform("region_size", sprite->get_region().uv.size());
            gpu.textures.bind(DIFFUSE_TEXTURE_INDEX, sprite->get_sprite_set()->get_texture());
        }

        size_t index_offset = 0;

        if (sprite->get_region().is_rotated)
        {
            index_offset = 12;
        }

        switch (triangle_mode)
        {
        case triangle_mode_e::BOTTOM_RIGHT:
            gpu.draw_elements(gpu_t::primitive_type_e::TRIANGLES, 3, index_buffer_type::DATA_TYPE, index_offset + 0);
            break;
        case triangle_mode_e::TOP_LEFT:
            gpu.draw_elements(gpu_t::primitive_type_e::TRIANGLES, 3, index_buffer_type::DATA_TYPE, index_offset + 3);
            break;
        case triangle_mode_e::TOP_RIGHT:
            gpu.draw_elements(gpu_t::primitive_type_e::TRIANGLES, 3, index_buffer_type::DATA_TYPE, index_offset + 6);
            break;
        case triangle_mode_e::BOTTOM_LEFT:
            gpu.draw_elements(gpu_t::primitive_type_e::TRIANGLES, 3, index_buffer_type::DATA_TYPE, index_offset + 9);
            break;
        case triangle_mode_e::BOTH:
            gpu.draw_elements(gpu_t::primitive_type_e::TRIANGLES, 6, index_buffer_type::DATA_TYPE, index_offset + 0);   //<-- CRASH HERE
            break;
        }

        gpu.textures.unbind(DIFFUSE_TEXTURE_INDEX);

		gpu.programs.pop();

		gpu.buffers.pop(gpu_t::buffer_target_e::ELEMENT_ARRAY);
		gpu.buffers.pop(gpu_t::buffer_target_e::ARRAY);

		gpu.blend.pop_state();

        gui_node_t::on_render_begin(world_matrix, view_projection_matrix);
	}
}