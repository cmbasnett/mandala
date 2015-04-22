#pragma once

//mandala
#include "gui_node.hpp"
#include "input_event.hpp"

namespace mandala
{
    struct gui_scroll_t : gui_node_t
    {
        typedef vec2_t scroll_location_type;
        typedef aabb2_t scroll_extents_type;

        virtual void on_input_event(input_event_t& input_event) override;
        virtual void tick(float32_t dt) override;
        virtual void render(const mat4_t& world_matrix, const mat4_t& view_projection_matrix) override;
		virtual void clean() override;

        inline const scroll_location_type get_scroll_location() const { return scroll_location; }
        inline const scroll_extents_type& get_scroll_extents() const { return scroll_extents; }
        inline bool get_is_scrolling() const { return is_scrolling; }

        inline void set_scroll_location(const scroll_location_type& scroll_location, bool should_interpolate = true);
        inline void set_scroll_extents(const scroll_extents_type& scroll_extents) { this->scroll_extents = scroll_extents; dirty(); }

    private:
        scroll_location_type scroll_location_target;
        scroll_location_type scroll_location;
        scroll_extents_type scroll_extents;
        bool is_scrolling = false;
        input_event_t::touch_t::touch_id_type touch_id = 0;	//TODO: figure out what to do with this
    };
};
