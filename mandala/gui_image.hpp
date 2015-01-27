#pragma once

//mandala
#include "sprite.hpp"
#include "gui_node.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "gpu_vertices.hpp"

namespace mandala
{
    struct gui_image_t : public gui_node_t
    {
        enum class triangle_mode_e
        {
            bottom_right,
            top_left,
            top_right,
            bottom_left,
            both
        };

        typedef aabb2_t uv_type;

		static const size_t vertex_count = 4;
		static const size_t index_count = 4;

        typedef position_texcoord_vertex_t vertex_type;
        typedef index_type<index_count>::type index_type;
        typedef vertex_buffer_t<vertex_type> vertex_buffer_type;
        typedef index_buffer_t<index_type> index_buffer_type;

		gui_image_t();

		const sprite_t& get_sprite() const { return sprite; }
		bool get_is_autosized_to_texture() const { return is_autosized_to_texture; }

		void set_sprite(const sprite_t& sprite) { this->sprite = sprite; _is_dirty = true; }
		void set_is_autosized_to_texture(bool is_autosized_to_texture) { this->is_autosized_to_texture = is_autosized_to_texture; _is_dirty = true; }
        void set_triangle_mode(triangle_mode_e triangle_mode) { this->triangle_mode = triangle_mode; _is_dirty = true; }

	private:
		std::shared_ptr<vertex_buffer_type> vertex_buffer;
		std::shared_ptr<index_buffer_type> index_buffer;
		sprite_t sprite;
		bool is_autosized_to_texture = false;
        triangle_mode_e triangle_mode = triangle_mode_e::both;

		virtual void render_override(mat4_t world_matrix, mat4_t view_projection_matrix) override;
		virtual void clean() override;
        virtual void on_input_event(input_event_t& input_event);
    };
}
