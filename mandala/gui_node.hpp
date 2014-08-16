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
		typedef aabb2_t bounds_type;
		typedef vec2_t size_type;

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

		std::shared_ptr<gui_node_t> parent() const { return _parent; }
		gui_dock_mode_e dock_mode() const { return _dock_mode; }
		gui_anchor_flags_type anchor_flags() const { return _anchor_flags; }
		const vec2_t& anchor_offset() const { return _anchor_offset; }
		const padding_t& padding() const { return _padding; }
		const padding_t& margin() const { return _margin; }
		const vec2_t& size() const { return _size; }
		const vec4_t& color() const { return _color; }
		const bounds_type& bounds() const { return _bounds; }
		const bool is_dirty() const { return _is_dirty; }
		const bool is_hidden() const { return _is_hidden; }

		void set_dock_mode(gui_dock_mode_e dock_mode);
		void set_anchor_flags(gui_anchor_flags_type anchor_flags);
		void set_anchor_offset(const vec2_t& anchor_offset);
		void set_padding(const padding_t& padding);
		void set_margin(const padding_t& margin);
		void set_size(const vec2_t& size);
		void set_color(const vec4_t& color);
		void set_bounds(const bounds_type& bounds);
		void set_is_hidden(bool is_hidden);

        std::vector<std::shared_ptr<gui_node_t>> children;

		//TODO: move trace logic into "layout" class
        static bool trace(std::shared_ptr<gui_node_t> node, trace_args_t args, trace_result_t& result);

        virtual void render(mat4_t world_matrix, mat4_t view_projection_matrix);
		virtual void clean();
		virtual void on_input_event(input_event_t& input_event);

        bool has_children() const { return children.size() > 0; }
		bool has_parent() const { return _parent.get() != nullptr; }

		void orphan();
        void adopt(std::shared_ptr<gui_node_t> child);

	private:
		std::shared_ptr<gui_node_t> _parent;
		gui_dock_mode_e _dock_mode = gui_dock_mode_e::none;
		gui_anchor_flags_type _anchor_flags = (gui_anchor_flag_top | gui_anchor_flag_left);
		vec2_t _anchor_offset;
		padding_t _padding;
		padding_t _margin;
		size_type _size;
		vec4_t _color = vec4_t(1.0f);
		bounds_type _bounds;
		bool _is_hidden = false;

	protected:
		bool _is_dirty = true;
	};
}
