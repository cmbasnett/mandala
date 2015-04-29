#pragma once

//std
#include <vector>

//boost
#include <boost\shared_ptr.hpp>
#include <boost\enable_shared_from_this.hpp>

//mandala
#include "aabb.hpp"
#include "circle.hpp"
#include "padding.hpp"
#include "range.hpp"

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

    enum class gui_size_mode_e
    {
        absolute,
        relative,
        inherit
    };

    struct gui_node_t : public boost::enable_shared_from_this<gui_node_t>
    {
        typedef aabb2_t bounds_type;
        typedef vec2_t size_type;
        typedef rgba_type color_type;

        const boost::shared_ptr<gui_node_t>& get_parent() const { return parent; }
        gui_dock_mode_e get_dock_mode() const { return dock_mode; }
        gui_anchor_flags_type get_anchor_flags() const { return anchor_flags; }
        const vec2_t& get_anchor_offset() const { return anchor_offset; }
        const padding_t& get_padding() const { return padding; }
        const padding_t& get_margin() const { return margin; }
        const gui_size_mode_e get_size_mode(bool is_recursive = true) const;
        const size_type& get_size() const { return size; }

        const color_type& get_color() const { return color; }
		const bounds_type& get_bounds() const { return bounds; }
        bool get_is_dirty() const { return is_dirty; }
		bool get_is_hidden() const { return is_hidden; }
		const std::vector<boost::shared_ptr<gui_node_t>>& get_children() const { return children; }

		void set_dock_mode(gui_dock_mode_e dock_mode) { this->dock_mode = dock_mode; dirty(); }
		void set_anchor_flags(gui_anchor_flags_type anchor_flags)
        {
            this->anchor_flags = anchor_flags; dirty();
        }
		void set_anchor_offset(const vec2_t& anchor_offset) { this->anchor_offset = anchor_offset; dirty(); }
        void set_padding(const padding_t& padding) { this->padding = padding; dirty(); }
		void set_margin(const padding_t& margin) { this->margin = margin; dirty(); }
        void set_size(const vec2_t& size, gui_size_mode_e size_mode = gui_size_mode_e::absolute);
        void set_color(const color_type& color) { this->color = color; dirty(); }
        void set_bounds(const bounds_type& bounds) { this->bounds = bounds; dirty(); }
        void set_is_hidden(bool is_hidden) { this->is_hidden = is_hidden; dirty(); }
        void set_should_clip(bool should_clip) { this->should_clip = should_clip; }

        virtual void render(const mat4_t& world_matrix, const mat4_t& view_projection_matrix);
        virtual void on_render(const mat4_t& world_matrix, const mat4_t& view_projection_matrix);
        virtual void on_render_begin(const mat4_t& world_matrix, const mat4_t& view_projection_matrix);
        virtual void on_render_end(const mat4_t& world_matrix, const mat4_t& view_projection_matrix) { }
        virtual void on_clean_begin() { }
        virtual void on_clean_end() { }
        virtual void tick(float32_t dt);
        virtual void on_input_event(input_event_t& input_event);

        void clean();

        bool has_children() const { return !children.empty(); }
		bool has_parent() const { return parent.get() != nullptr; }

        void dirty();
		void orphan();
        void adopt(const boost::shared_ptr<gui_node_t>& child);

    private:
        boost::shared_ptr<gui_node_t> parent;
        std::vector<boost::shared_ptr<gui_node_t>> children;
        gui_dock_mode_e dock_mode = gui_dock_mode_e::none;
        gui_anchor_flags_type anchor_flags = (gui_anchor_flag_top | gui_anchor_flag_left);
        vec2_t anchor_offset;
        padding_t padding;
        padding_t margin;
        gui_size_mode_e size_mode = gui_size_mode_e::absolute;
        size_type size;
        size_type desired_size;
        range_<size_type> size_range;
        color_type color = color_type(1.0f);
        bounds_type bounds;
        bool is_hidden = false;
        bool should_clip = false;
		bool has_focus = false;

	protected:
		bool is_dirty = true;
	};
}
