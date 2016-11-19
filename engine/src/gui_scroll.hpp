#pragma once

//naga
#include "gui_node.hpp"
#include "input_event.hpp"

namespace naga
{
    struct gui_scroll : gui_node
    {
        typedef vec2 scroll_location_type;
        typedef aabb2 scroll_extents_type;

        virtual bool on_input_event_begin(input_event_t& input_event) override;
        virtual void on_tick_end(f32 dt) override;
        virtual void on_render_begin(mat4& world_matrix, mat4& view_projection_matrix) override;
        virtual void on_clean_end() override;

        inline const scroll_location_type get_scroll_location() const { return scroll_location; }
        inline bool get_is_scrolling() const { return is_scrolling; }

        void set_scroll_location(const scroll_location_type& scroll_location, bool should_interpolate = true);

        scroll_extents_type scroll_extents;

    private:
        scroll_location_type scroll_location_target;
        scroll_location_type scroll_location;
        bool is_scrolling = false;
        input_event_t::touch_t::touch_id_type touch_id = 0;    //TODO: figure out what to do with this
    };
};
