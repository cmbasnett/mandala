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

namespace naga
{
	struct InputEvent;
    struct GUILayout;

    typedef u8 GUIAnchorFlagsType;

    enum : GUIAnchorFlagsType
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

    enum class GUIDockMode
    {
        NONE,
        BOTTOM,
        FILL,
        LEFT,
        RIGHT,
        TOP
    };

    enum class GUIVisibility
    {
        OMIT,
        HIDDEN,
        VISIBLE
    };

	struct GUINode : public boost::enable_shared_from_this<GUINode>
    {
        typedef AABB2 BoundsType;
        typedef vec2 SizeType;
        typedef vec4 ColorType;

        const std::string& get_name() const { return name; }
        const boost::shared_ptr<GUINode>& get_root() const { return root; }
		const boost::shared_ptr<GUINode>& get_parent() const { return parent; }
        GUIDockMode get_dock_mode() const { return dock_mode; }
        GUIAnchorFlagsType get_anchor_flags() const { return anchor_flags; }
        const vec2& get_anchor_offset() const { return anchor_offset; }
		const Padding& get_padding() const { return padding; }
		const Padding& get_margin() const { return margin; }
        const GUISizeModes& get_size_modes() const { return size_modes; }
        const GUISizeModes get_size_modes_real() const;
        const SizeType& get_desired_size() const { return desired_size; }
        const SizeType& get_size() const { return size; }

        const ColorType& get_color() const { return color; }
        const BoundsType& get_bounds() const { return bounds; }
        bool get_is_dirty() const { return is_dirty; }
        GUIVisibility get_visibility() const { return visibility; }
        bool get_should_clip() const { return should_clip; }
		const std::vector<boost::shared_ptr<GUINode>>& get_children() const { return children; }

        void set_dock_mode(GUIDockMode dock_mode) { this->dock_mode = dock_mode; dirty(); }
        void set_anchor_flags(GUIAnchorFlagsType anchor_flags) { this->anchor_flags = anchor_flags; dirty(); }
        void set_anchor_offset(const vec2& anchor_offset) { this->anchor_offset = anchor_offset; dirty(); }
		void set_padding(const Padding& padding) { this->padding = padding; dirty(); }
		void set_margin(const Padding& margin) { this->margin = margin; dirty(); }
        void set_size(const SizeType& size) { this->desired_size = size; dirty(); }
        void set_size_modes(const GUISizeModes size_modes) { this->size_modes = size_modes; dirty(); }
        void set_color(const ColorType& color) { this->color = color; dirty(); }
        void set_bounds(const BoundsType& bounds) { this->bounds = bounds; dirty(); }
        void set_visibility(GUIVisibility visibility) { this->visibility = visibility; dirty(); }
        void set_should_clip(bool should_clip) { this->should_clip = should_clip; }

        virtual void on_clean_begin() { }
        virtual void on_clean_end() { }
        virtual void on_render_begin(mat4& world_matrix, mat4& view_projection_matrix) { }
        virtual void on_render_end(mat4& world_matrix, mat4& view_projection_matrix) { }
        virtual void on_tick_begin(f32 dt) { }
        virtual void on_tick_end(f32 dt) { }
		virtual bool on_input_event_begin(InputEvent& input_event) { return false; }
		virtual bool on_input_event_end(InputEvent& input_event) { return false; }

        void clean();
        void tick(f32 dt);
        void render(mat4 world_matrix, mat4 view_projection_matrix);
		bool on_input_event(InputEvent& input_event);

        bool has_children() const { return !children.empty(); }
        bool has_parent() const { return parent.get() != nullptr; }

        void dirty();
        void orphan();
		void adopt(const boost::shared_ptr<GUINode>& child);

    private:
		std::string name;
		boost::shared_ptr<GUINode> root;
		boost::shared_ptr<GUINode> parent;
		std::vector<boost::shared_ptr<GUINode>> children;
        GUIDockMode dock_mode = GUIDockMode::NONE;
        GUIAnchorFlagsType anchor_flags = (GUI_ANCHOR_FLAG_TOP | GUI_ANCHOR_FLAG_LEFT);
        SizeType anchor_offset;
		Padding padding;
		Padding margin;
        SizeType size;
        SizeType desired_size;
        ColorType color = ColorType(1.0f);
        BoundsType bounds;
        GUIVisibility visibility = GUIVisibility::VISIBLE;
        bool should_clip = false;
        bool has_focus = false;
        GUISizeModes size_modes;
        boost::shared_ptr<GUILayout> layout;
		bool should_wrap_to_content_width;

    protected:
        bool is_dirty = true;
    };
}
