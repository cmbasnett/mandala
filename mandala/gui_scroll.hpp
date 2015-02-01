#pragma once

//mandala
#include "gui_node.hpp"
#include "input_event.hpp"

namespace mandala
{
    struct gui_scroll_t : gui_node_t
    {
        typedef vec2_t scroll_position_type;
        typedef aabb2_t scroll_extents_type;

        virtual void on_input_event(input_event_t& input_event) override;
        virtual void tick(float32_t dt) override;
        virtual void render(mat4_t world_matrix, mat4_t view_projection_matrix) override;
		virtual void clean() override;

        inline const scroll_position_type get_scroll_position() const { return scroll_position; }
        inline const scroll_extents_type& get_scroll_extents() const { return scroll_extents; }
        inline bool get_is_scrolling() const { return is_scrolling; }

        inline void set_scroll_position(const scroll_position_type& scroll_position) { this->scroll_position = scroll_position; dirty(); }
        inline void set_scroll_extents(const scroll_extents_type& scroll_extents) { this->scroll_extents = scroll_extents; dirty(); }

    private:
        scroll_position_type scroll_position;
        scroll_extents_type scroll_extents;
        bool is_scrolling = false;
        input_event_t::touch_t::touch_id_type touch_id = 0;	//TODO: figure out what to do with this
    };
};
