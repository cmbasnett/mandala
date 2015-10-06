#pragma once

//mandala
#include "sprite.hpp"
#include "gui_node.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "gui_image_gpu_program.hpp"

namespace mandala
{
    struct gui_image_t : public gui_node_t
    {
        enum class triangle_mode_e
        {
            BOTTOM_RIGHT,
            TOP_LEFT,
            TOP_RIGHT,
            BOTTOM_LEFT,
            SLICE,
            BOTH
        };

        static const size_t VERTEX_COUNT = 16;
        static const size_t INDEX_COUNT = 54;
        
        typedef gui_image_gpu_program_t::vertex_type vertex_type;
        typedef index_type<INDEX_COUNT>::type index_type;
        typedef vertex_buffer_t<vertex_type> vertex_buffer_type;
        typedef index_buffer_t<index_type> index_buffer_type;

        gui_image_t();

        const boost::optional<sprite_t>& get_sprite() const { return sprite; }
        bool get_is_autosized_to_sprite() const { return is_autosized_to_sprite; }
        triangle_mode_e get_triangle_mode() const { return triangle_mode; }
        const vec2& get_texcoord_scale() const { return texcoord_scale; }
        const vec2& get_texcoord_origin() const { return texcoord_origin; }
        f32 get_texcoord_rotation() const { return texcoord_rotation; }
        const padding_f32& get_slice_padding() const { return slice_padding; }

        void set_sprite(boost::optional<sprite_t> sprite);
        void set_is_autosized_to_sprite(bool is_autosized_to_sprite);
        void set_triangle_mode(triangle_mode_e triangle_mode) { this->triangle_mode = triangle_mode; }
        void set_texcoord_scale(const vec2& texcoord_scale) { this->texcoord_scale = texcoord_scale; }
        void set_texcoord_origin(const vec2& texcoord_origin) { this->texcoord_origin = texcoord_origin; }
        void set_texcoord_rotation(f32 texcoord_rotation) { this->texcoord_rotation = texcoord_rotation; }
		void set_slice_padding(const padding_f32& slice_padding) { this->slice_padding = slice_padding; }

    private:
        boost::shared_ptr<vertex_buffer_type> vertex_buffer;
        boost::shared_ptr<index_buffer_type> index_buffer;
        boost::optional<sprite_t> sprite;
        bool is_autosized_to_sprite = false;
        triangle_mode_e triangle_mode = triangle_mode_e::BOTH;
        vec2 texcoord_scale = vec2(1.0f);
        vec2 texcoord_origin = vec2(0.5f);
        vec2 texcoord_offset;
        f32 texcoord_rotation = 0.0f;
        padding_f32 slice_padding;

        virtual void on_render_begin(mat4& world_matrix, mat4& view_projection_matrix) override;
        virtual void on_clean_end() override;
    };
}
