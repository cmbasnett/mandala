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
            0, 1, 2, 
            0, 2, 3, 
            1, 2, 3, 
            1, 3, 0,
            0, 4, 6, 4, 7, 6,
            4, 5, 7, 5, 8, 7,
            5, 1, 8, 1, 9, 8,
            6, 7, 10, 7, 11, 10,
            7, 8, 11, 8, 12, 11,
            8, 9, 12, 9, 13, 12,
            10, 11, 3, 11, 14, 3,
            11, 12, 14, 12, 15, 14,
            12, 13, 15, 13, 2, 15
        };
        index_buffer->data(indices, gpu_t::buffer_usage_e::STATIC_DRAW);
    }

    void gui_image_t::on_clean_end()
    {
        //need to take size into consideration! (size is frame size, don't worry)
        const auto& size = get_size();
        auto slice_uv = slice_padding;

        slice_uv.bottom /= sprite->get_region()->source_size.y;
        slice_uv.left /= sprite->get_region()->source_size.x;
        slice_uv.right /= sprite->get_region()->source_size.x;
        slice_uv.top /= sprite->get_region()->source_size.y;

        //TODO: find a better way to generalize this so it's less of a mess
        auto vertices = {
            vertex_type(vec3_t(0, 0, 0), vec2_t(0, 0)),
            vertex_type(vec3_t(size.x, 0, 0), vec2_t(1, 0)),
            vertex_type(vec3_t(size.x, size.y, 0), vec2_t(1, 1)),
            vertex_type(vec3_t(0, size.y, 0), vec2_t(0, 1)),

            vertex_type(vec3_t(0 + slice_padding.left, 0, 0), vec2_t(slice_uv.left, 0)),
            vertex_type(vec3_t(size.x - slice_padding.right, 0, 0), vec2_t(1.0f - slice_uv.right, 0)),

            vertex_type(vec3_t(0, slice_padding.bottom, 0), vec2_t(0, slice_uv.bottom)),
            vertex_type(vec3_t(slice_padding.left, slice_padding.bottom, 0), vec2_t(slice_uv.left, slice_uv.bottom)),
            vertex_type(vec3_t(size.x - slice_padding.right, slice_padding.bottom, 0), vec2_t(1.0f - slice_uv.right, slice_uv.bottom)),
            vertex_type(vec3_t(size.x, slice_padding.bottom, 0), vec2_t(1.0f, slice_uv.bottom)),

            vertex_type(vec3_t(0, size.y - slice_padding.top, 0), vec2_t(0, 1.0f - slice_uv.top)),
            vertex_type(vec3_t(slice_padding.left, size.y - slice_padding.top, 0), vec2_t(slice_uv.left, 1.0f - slice_uv.top)),
            vertex_type(vec3_t(size.x - slice_padding.right, size.y - slice_padding.top, 0), vec2_t(1.0f - slice_uv.right, 1.0f - slice_uv.top)),
            vertex_type(vec3_t(size.x, size.y - slice_padding.top, 0), vec2_t(1.0f, 1.0f - slice_uv.top)),

            vertex_type(vec3_t(0 + slice_padding.left, size.y, 0), vec2_t(slice_uv.left, 1.0)),
            vertex_type(vec3_t(size.x - slice_padding.right, size.y, 0), vec2_t(1.0f - slice_uv.right, 1.0f)),
        };

        vertex_buffer->data(vertices, gpu_t::buffer_usage_e::STATIC_DRAW);
    }

    void gui_image_t::set_sprite(boost::optional<sprite_t> sprite)
    {
        this->sprite = sprite;

        if (sprite && is_autosized_to_sprite)
        {
            set_size_modes(gui_size_modes_t(gui_size_mode_e::ABSOLUTE, gui_size_mode_e::ABSOLUTE));
            set_size(static_cast<size_type>(sprite->get_region()->source_size));
        }
    }

    void gui_image_t::set_is_autosized_to_sprite(bool is_autosized_to_sprite)
    {
        this->is_autosized_to_sprite = is_autosized_to_sprite;

        set_sprite(sprite);
    }

    void gui_image_t::on_render_begin(mat4_t& world_matrix, mat4_t& view_projection_matrix)
    {
        if (sprite)
        {
            const auto& region = sprite->get_region();

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

            const auto& min = get_bounds().min;
            const auto& size = get_size();

            static const auto DIFFUSE_TEXTURE_INDEX = 0;

            gpu.set_uniform("diffuse_texture", DIFFUSE_TEXTURE_INDEX);
            gpu.set_uniform("world_matrix", world_matrix * glm::translate(min.x, min.y, 0.0f));
            gpu.set_uniform("view_projection_matrix", view_projection_matrix);
            gpu.set_uniform("color", get_color());
            gpu.set_uniform("texcoord_origin", texcoord_origin);
            gpu.set_uniform("texcoord_scale", texcoord_scale);
            gpu.set_uniform("rotation", (region->is_rotated ? 1.57079633f : 0) + texcoord_rotation); //TODO: make this a bit more readable
            gpu.set_uniform("region_min", region->frame_uv.min);
            gpu.set_uniform("region_max", region->frame_uv.max);
            gpu.set_uniform("frame_min", region->uv.min);
            gpu.set_uniform("frame_max", region->uv.max);

            gpu.textures.bind(DIFFUSE_TEXTURE_INDEX, sprite->get_sprite_set()->get_texture());

            switch (triangle_mode)
            {
            case triangle_mode_e::BOTTOM_RIGHT:
                gpu.draw_elements(gpu_t::primitive_type_e::TRIANGLES, 3, index_buffer_type::DATA_TYPE, 0);
                break;
            case triangle_mode_e::TOP_LEFT:
                gpu.draw_elements(gpu_t::primitive_type_e::TRIANGLES, 3, index_buffer_type::DATA_TYPE, 3);
                break;
            case triangle_mode_e::TOP_RIGHT:
                gpu.draw_elements(gpu_t::primitive_type_e::TRIANGLES, 3, index_buffer_type::DATA_TYPE, 6);
                break;
            case triangle_mode_e::BOTTOM_LEFT:
                gpu.draw_elements(gpu_t::primitive_type_e::TRIANGLES, 3, index_buffer_type::DATA_TYPE, 9);
                break;
            case triangle_mode_e::BOTH:
                gpu.draw_elements(gpu_t::primitive_type_e::TRIANGLES, 6, index_buffer_type::DATA_TYPE, 0);
                break;
            case triangle_mode_e::SLICE:
                gpu.draw_elements(gpu_t::primitive_type_e::TRIANGLES, 54, index_buffer_type::DATA_TYPE, 12);
                break;
            }

            gpu.textures.unbind(DIFFUSE_TEXTURE_INDEX);

            gpu.programs.pop();

            gpu.buffers.pop(gpu_t::buffer_target_e::ELEMENT_ARRAY);
            gpu.buffers.pop(gpu_t::buffer_target_e::ARRAY);

            gpu.blend.pop_state();
        }

        gui_node_t::on_render_begin(world_matrix, view_projection_matrix);
    }
}
