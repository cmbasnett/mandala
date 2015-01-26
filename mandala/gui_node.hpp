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

	enum class gui_resize_mode_e
	{
	};

	struct gui_node_t
	{
		typedef aabb2_t bounds_type;
		typedef vec2_t size_type;
        typedef rgba_type color_type;

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

		const std::shared_ptr<gui_node_t>& parent() const { return _parent; }
		gui_dock_mode_e dock_mode() const { return _dock_mode; }
		gui_anchor_flags_type anchor_flags() const { return _anchor_flags; }
		const vec2_t& anchor_offset() const { return _anchor_offset; }
		const padding_t& padding() const { return _padding; }
		const padding_t& margin() const { return _margin; }
		const vec2_t& size() const { return _size; }
        const color_type& color() const { return _color; }
		const bounds_type& bounds() const { return _bounds; }
		bool is_dirty() const;
		bool is_hidden() const { return _is_hidden; }
		const std::vector<std::shared_ptr<gui_node_t>>& children() const { return _children; }

        void dirty() { _is_dirty = true; }	//TODO: cascade dirtyiness upwards if parent relies on this element for sizing
		void set_dock_mode(gui_dock_mode_e dock_mode) { _dock_mode = dock_mode; _is_dirty = true; }
		void set_anchor_flags(gui_anchor_flags_type anchor_flags) { _anchor_flags = anchor_flags; _is_dirty = true; }
		void set_anchor_offset(const vec2_t& anchor_offset) { _anchor_offset = anchor_offset; _is_dirty = true; }
		void set_padding(const padding_t& padding) { _padding = padding; _is_dirty = true; }
		void set_margin(const padding_t& margin) { _margin = margin; _is_dirty = true; }
		void set_size(const vec2_t& size) { _size = size; _is_dirty = true; }
        void set_color(const color_type& color) { _color = color; _is_dirty = true; }
		void set_bounds(const bounds_type& bounds) { _bounds = bounds; _is_dirty = true; }
		void set_is_hidden(bool is_hidden) { _is_hidden = is_hidden; _is_dirty = true; }

		//TODO: move trace logic into "layout" class
        static bool trace(std::shared_ptr<gui_node_t> node, trace_args_t args, trace_result_t& result);

        virtual void render(mat4_t world_matrix, mat4_t view_projection_matrix);

        virtual void render_override(mat4_t world_matrix, mat4_t view_projection_matrix);
		virtual void clean();
        virtual void tick(float32_t dt);
        virtual void on_input_event(input_event_t& input_event);

        bool has_children() const { return !_children.empty(); }
		bool has_parent() const { return _parent.get() != nullptr; }

		void orphan();
        void adopt(std::shared_ptr<gui_node_t> child);

    private:
        std::shared_ptr<gui_node_t> _parent;
        std::vector<std::shared_ptr<gui_node_t>> _children;
        gui_dock_mode_e _dock_mode = gui_dock_mode_e::none;
        gui_anchor_flags_type _anchor_flags = (gui_anchor_flag_top | gui_anchor_flag_left);
        vec2_t _anchor_offset;
        padding_t _padding;
        padding_t _margin;
        size_type _size;
        color_type _color = color_type(1.0f);
        bounds_type _bounds;
        bool _is_hidden = false;
        bool _should_clip = false;
		bool _has_focus = false;

	protected:
		bool _is_dirty = true;
	};
}
