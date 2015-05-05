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
            BOTTOM_RIGHT,
            TOP_LEFT,
            TOP_RIGHT,
            BOTTOM_LEFT,
            BOTH
        };

        enum class texcoord_mode_e
        {
            STRETCH_TO_FIT,
            REPEAT
        };

		static const size_t VERTEX_COUNT = 4;
		static const size_t INDEX_COUNT = 4;
        
        typedef basic_gpu_vertex_t vertex_type;
        typedef index_type<INDEX_COUNT>::type index_type;
        typedef vertex_buffer_t<vertex_type> vertex_buffer_type;
        typedef index_buffer_t<index_type> index_buffer_type;

		gui_image_t();

		const boost::optional<sprite_t>& get_sprite() const { return sprite; }
		bool get_is_autosized_to_sprite() const { return is_autosized_to_sprite; }
        triangle_mode_e get_triangle_mode() const { return triangle_mode; }

        void set_sprite(boost::optional<sprite_t> sprite);
        void set_is_autosized_to_sprite(bool is_autosized_to_sprite);
        void set_triangle_mode(triangle_mode_e triangle_mode) { this->triangle_mode = triangle_mode; }

	private:
		boost::shared_ptr<vertex_buffer_type> vertex_buffer;
		boost::shared_ptr<index_buffer_type> index_buffer;
		boost::optional<sprite_t> sprite;
		bool is_autosized_to_sprite = false;
        triangle_mode_e triangle_mode = triangle_mode_e::BOTH;

		virtual void on_render_begin(mat4_t& world_matrix, mat4_t& view_projection_matrix) override;
        virtual void on_input_event(input_event_t& input_event) override;
        virtual void on_clean_end() override;
    };
}
