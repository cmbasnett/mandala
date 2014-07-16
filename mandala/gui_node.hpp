#pragma once

//std
#include <vector>
#include <memory>

//mandala
#include "mandala.hpp"
#include "aabb.hpp"
#include "circle.hpp"
#include "padding.hpp"

namespace mandala
{
	struct input_event_t;

	struct gui_node_t
	{
		typedef uint8_t anchor_flags_type;

        enum : anchor_flags_type
        {
            anchor_flag_none = 0,
            anchor_flag_bottom = (1 << 0),
            anchor_flag_left = (1 << 1),
            anchor_flag_right = (1 << 2),
            anchor_flag_top = (1 << 3),
            anchor_flag_vertical = (anchor_flag_bottom | anchor_flag_top),
            anchor_flag_horizontal = (anchor_flag_left | anchor_flag_right),
            anchor_flag_all = (anchor_flag_vertical | anchor_flag_horizontal)
        };

		enum class dock_mode_e : uint8_t
		{
            none,
            bottom,
            fill,
            left,
            right,
			top
		};

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
		dock_mode_e dock_mode = dock_mode_e::none;
        anchor_flags_type anchor_flags = (anchor_flag_top | anchor_flag_left);
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
		virtual bool clean();
		virtual void on_input_event(input_event_t& input_event);

        inline bool has_children() const { return children.size() > 0; }
		inline bool has_parent() const { return parent.get() != nullptr; }

		void orphan();
        void adopt(std::shared_ptr<gui_node_t> child);
	};
};
