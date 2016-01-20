#pragma once

//std
#include <vector>

//boost
#include <boost\shared_ptr.hpp>
#include <boost\enable_shared_from_this.hpp>

//naga
#include "aabb.hpp"
#include "circle.hpp"
#include "padding.hpp"
#include "range.hpp"
#include "gui_size.hpp"
#include "hash.hpp"

namespace naga
{
    struct input_event_t;
    struct gui_layout;

    typedef u8 gui_anchor_flags_type;

    enum : gui_anchor_flags_type
    {
        GUI_ANCHOR_FLAG_NONE = 0,
        GUI_ANCHOR_FLAG_BOTTOM = (1 << 0),
        GUI_ANCHOR_FLAG_LEFT = (1 << 1),
        GUI_ANCHOR_FLAG_RIGHT = (1 << 2),
        GUI_ANCHOR_FLAG_TOP = (1 << 3),
        GUI_ANCHOR_FLAG_VERTICAL = (GUI_ANCHOR_FLAG_BOTTOM | GUI_ANCHOR_FLAG_TOP),
        GUI_ANCHOR_FLAG_HORIZONTAL = (GUI_ANCHOR_FLAG_LEFT | GUI_ANCHOR_FLAG_RIGHT),
        GUI_ANCHOR_FLAG_ALL = (GUI_ANCHOR_FLAG_VERTICAL | GUI_ANCHOR_FLAG_HORIZONTAL)
    };

    enum class gui_dock_mode
    {
        NONE,
        BOTTOM,
        FILL,
        LEFT,
        RIGHT,
        TOP
    };

    enum class gui_visiblity
    {
        OMIT,
        HIDDEN,
        VISIBLE
    };

    struct gui_node : public boost::enable_shared_from_this<gui_node>
    {
        typedef aabb2 bounds_type;
        typedef vec2 size_type;
        typedef vec4 color_type;

        const hash& get_id() const { return id; }
        const boost::shared_ptr<gui_node>& get_root() const { return root; }
        const boost::shared_ptr<gui_node>& get_parent() const { return parent; }
        gui_dock_mode get_dock_mode() const { return dock_mode; }
        gui_anchor_flags_type get_anchor_flags() const { return anchor_flags; }
        const vec2& get_anchor_offset() const { return anchor_offset; }
        const padding& get_padding() const { return padding; }
        const padding& get_margin() const { return margin; }
        const gui_size_modes_t& get_size_modes() const { return size_modes; }
        const gui_size_modes_t get_size_modes_real() const;
        const size_type& get_desired_size() const { return desired_size; }
        const size_type& get_size() const { return size; }

        const color_type& get_color() const { return color; }
        const bounds_type& get_bounds() const { return bounds; }
        bool get_is_dirty() const { return is_dirty; }
        gui_visiblity get_visibility() const { return visibility; }
        bool get_should_clip() const { return should_clip; }
        const std::vector<boost::shared_ptr<gui_node>>& get_children() const { return children; }

        void set_dock_mode(gui_dock_mode dock_mode) { this->dock_mode = dock_mode; dirty(); }
        void set_anchor_flags(gui_anchor_flags_type anchor_flags) { this->anchor_flags = anchor_flags; dirty(); }
        void set_anchor_offset(const vec2& anchor_offset) { this->anchor_offset = anchor_offset; dirty(); }
        void set_padding(const padding& padding) { this->padding = padding; dirty(); }
        void set_margin(const padding& margin) { this->margin = margin; dirty(); }
        void set_size(const size_type& size) { this->desired_size = size; dirty(); }
        void set_size_modes(const gui_size_modes_t size_modes) { this->size_modes = size_modes; dirty(); }
        void set_color(const color_type& color) { this->color = color; dirty(); }
        void set_bounds(const bounds_type& bounds) { this->bounds = bounds; dirty(); }
        void set_visibility(gui_visiblity visibility) { this->visibility = visibility; dirty(); }
        void set_should_clip(bool should_clip) { this->should_clip = should_clip; }

        virtual void on_clean_begin() { }
        virtual void on_clean_end() { }
        virtual void on_render_begin(mat4& world_matrix, mat4& view_projection_matrix) { }
        virtual void on_render_end(mat4& world_matrix, mat4& view_projection_matrix) { }
        virtual void on_tick_begin(f32 dt) { }
        virtual void on_tick_end(f32 dt) { }
        virtual bool on_input_event_begin(input_event_t& input_event) { return false; }
        virtual bool on_input_event_end(input_event_t& input_event) { return false; }

        void clean();
        void tick(f32 dt);
        void render(mat4 world_matrix, mat4 view_projection_matrix);
        bool on_input_event(input_event_t& input_event);

        bool has_children() const { return !children.empty(); }
        bool has_parent() const { return parent.get() != nullptr; }

        void dirty();
        void orphan();
        void adopt(const boost::shared_ptr<gui_node>& child);

    private:
        hash id;
        boost::shared_ptr<gui_node> root;
        boost::shared_ptr<gui_node> parent;
        std::vector<boost::shared_ptr<gui_node>> children;
        gui_dock_mode dock_mode = gui_dock_mode::NONE;
        gui_anchor_flags_type anchor_flags = (GUI_ANCHOR_FLAG_TOP | GUI_ANCHOR_FLAG_LEFT);
        size_type anchor_offset;
        padding_f32 padding;
        padding_f32 margin;
        size_type size;
        size_type desired_size;
        color_type color = color_type(1.0f);
        bounds_type bounds;
        gui_visiblity visibility = gui_visiblity::VISIBLE;
        bool should_clip = false;
        bool has_focus = false;
        gui_size_modes_t size_modes;
        boost::shared_ptr<gui_layout> layout;

    protected:
        bool is_dirty = true;
    };
}
