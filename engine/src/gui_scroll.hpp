#pragma once

//naga
#include "gui_node.hpp"
#include "input_event.hpp"

namespace naga
{
	struct GUIScroll : GUINode
    {
		virtual bool on_input_event_begin(InputEvent& input_event) override;
        virtual void on_tick_end(f32 dt) override;
        virtual void on_render_begin(mat4& world_matrix, mat4& view_projection_matrix) override;
        virtual void on_clean_end() override;

        inline const vec2 get_scroll_location() const { return scroll_location; }
        inline bool get_is_scrolling() const { return is_scrolling; }

		void set_scroll_location(const vec2& scroll_location, bool should_interpolate = true);

        AABB2 scroll_extents;

    private:
		vec2 scroll_location_target;
		vec2 scroll_location;
        bool is_scrolling = false;
		size_t touch_id = 0;    //TODO: figure out what to do with this
    };
};
