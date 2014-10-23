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

        inline const scroll_position_type scroll_position() const { return _scroll_position; }
        inline const scroll_extents_type& scroll_extents() const { return _scroll_extents; }
        inline bool is_scrolling() const { return _is_scrolling; }

        inline void set_scroll_position(const scroll_position_type& scroll_position) { _scroll_position = scroll_position; _is_dirty = true; }
        inline void set_scroll_extents(const scroll_extents_type& scroll_extents) { _scroll_extents = scroll_extents; _is_dirty = true; }

    private:
        scroll_position_type _scroll_position;
        scroll_extents_type _scroll_extents;
        bool _is_scrolling = false;
        input_event_t::touch_t::touch_id_type _touch_id = 0;
    };
};
