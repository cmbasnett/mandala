#pragma once

//std
#include <vector>
#include <memory>

//mandala
#include "aabb.hpp"
#include "circle.hpp"
#include "padding.hpp"

namespace mandala
{
	struct input_event_t;

	typedef uint8_t gui_anchor_flags_type;

	enum : gui_anchor_flags_type
	{
		gui_anchor_flag_none = 0,
		gui_anchor_flag_bottom = (1 << 0),
		gui_anchor_flag_left = (1 << 1),
		gui_anchor_flag_right = (1 << 2),
		gui_anchor_flag_top = (1 << 3),
		gui_anchor_flag_vertical = (gui_anchor_flag_bottom | gui_anchor_flag_top),
		gui_anchor_flag_horizontal = (gui_anchor_flag_left | gui_anchor_flag_right),
		gui_anchor_flag_all = (gui_anchor_flag_vertical | gui_anchor_flag_horizontal)
	};

	enum class gui_dock_mode_e
	{
		none,
		bottom,
		fill,
		left,
		right,
		top
	};

	struct gui_node_t
	{
		struct trace_args_t
		{
			circle_f32_t circle;
		};

		struct trace_result_t
		{
			bool did_hit = false;
			float32_t distance = 0.0f;
            std::vector<std::shared_ptr<gui_node_t>> nodes_hit;
		};

        std::shared_ptr<gui_node_t> parent;
        std::vector<std::shared_ptr<gui_node_t>> children;
		gui_dock_mode_e dock_mode = gui_dock_mode_e::none;
        gui_anchor_flags_type anchor_flags = (gui_anchor_flag_top | gui_anchor_flag_left);
		vec2_t anchor_offset;
		padding_t padding;
		padding_t margin;
		vec2_t size;
		vec4_t color = vec4_t(1.0f);
		aabb2_t bounds;
		bool is_dirty = true;
		float32_t rotation = 0.0f;
		bool is_hidden = false;

		//TODO: move trace logic into "layout" class
        static bool trace(std::shared_ptr<gui_node_t> node, trace_args_t args, trace_result_t& result);

        virtual void render(mat4_t world_matrix, mat4_t view_projection_matrix);
		virtual void clean();
		virtual void on_input_event(input_event_t& input_event);

        inline bool has_children() const { return children.size() > 0; }
		inline bool has_parent() const { return parent.get() != nullptr; }

		void orphan();
        void adopt(std::shared_ptr<gui_node_t> child);
	};
}
