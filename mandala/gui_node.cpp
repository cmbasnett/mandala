#pragma once

//std
#include <algorithm>

//glm
#include <glm\glm.hpp>
#include <glm\ext.hpp>

//mandala
#include "collision.hpp"
#include "input_event.hpp"
#include "gui_node.hpp"
#include "gpu.hpp"

#if defined(DEBUG)
#include "line_renderer.hpp"
#endif

namespace mandala
{
    void gui_node_t::orphan()
    {
        if (parent.get() == nullptr)
        {
            throw std::exception("cannot orphan a node with no parent");
        }

        auto parent_children_itr = std::find(parent->children.begin(), parent->children.end(), parent);

        if (parent_children_itr == parent->children.end())
        {
            throw std::exception("parent does not have this node as a child");
        }

        //remove node from parent's child list
        parent->children.erase(parent_children_itr);

        //TODO: might be able to forego dirtying parent removing this node doesn't
        // affect positioning of sibling elements

        //mark parent as dirty
        parent->dirty();

        //unparent
        parent = nullptr;

        //mark as dirty
        dirty();
    }

    void gui_node_t::adopt(const boost::shared_ptr<gui_node_t>& node)
    {
        if (node == nullptr || node.get() == this)
        {
            throw std::invalid_argument("");
        }

        if (node->has_parent())
        {
            //orphan node from parent
            node->orphan();
        }

        const auto children_itr = std::find(children.begin(), children.end(), node);

        if (children_itr != children.end())
        {
            //node is already a child
            throw std::exception();
        }

        //add node to child list
        children.push_back(node);

        //mark as dirty
        dirty();
    }

    void gui_node_t::clean_internal(aabb2_t& sibling_bounds)
    {
        auto absolute_size = desired_size;
        auto parent_margin = padding_t();

        switch (get_size_mode())
        {
        case gui_size_mode_e::relative:
            if (has_parent())   //TODO: doesn't work yet; parents not being set for some reason
            {
                absolute_size *= parent->bounds.size();
            }
            else
            {
                absolute_size *= sibling_bounds.size();
            }

            break;
        }

        switch (dock_mode)
        {
        case gui_dock_mode_e::bottom:
            bounds.min = sibling_bounds.min;
            bounds.max.x = sibling_bounds.max.x;
            bounds.max.y = sibling_bounds.min.y + absolute_size.y + padding.vertical();

            bounds -= margin;

            sibling_bounds.min.y += bounds.height() + margin.vertical();

            break;
        case gui_dock_mode_e::fill:
            bounds = sibling_bounds - margin;

            break;
        case gui_dock_mode_e::left:
            bounds.min = sibling_bounds.min;
            bounds.max.x = sibling_bounds.min.x + absolute_size.x + padding.horizontal();
            bounds.max.y = sibling_bounds.max.y;

            bounds -= margin;

            sibling_bounds.min.x += bounds.width() + margin.horizontal();

            break;
        case gui_dock_mode_e::right:
            bounds.min.x = sibling_bounds.max.x - absolute_size.x - padding.horizontal();
            bounds.min.y = sibling_bounds.min.y;
            bounds.max = sibling_bounds.max;

            bounds -= margin;

            sibling_bounds.max.x -= bounds.width() + margin.horizontal();

            break;
        case gui_dock_mode_e::top:
            bounds.min.x = sibling_bounds.min.x;
            bounds.min.y = sibling_bounds.max.y - absolute_size.y - padding.vertical();
            bounds.max = sibling_bounds.max;

            bounds -= margin;

            sibling_bounds.max.y -= bounds.height() + margin.vertical();

            break;
        default:
            bounds.min = vec2_t(0);
            bounds.max = absolute_size;

            vec2_t anchor_location;
            vec2_t anchor_translation;

            if ((anchor_flags & gui_anchor_flag_horizontal) == gui_anchor_flag_horizontal)
            {
                anchor_location.x = (sibling_bounds.max.x - sibling_bounds.min.x) / 2;
                anchor_translation.x = -((absolute_size.x / 2) + ((margin.right - margin.left) / 2));
            }
            else
            {
                if ((anchor_flags & gui_anchor_flag_left) == gui_anchor_flag_left)
                {
                    anchor_location.x = sibling_bounds.min.x;
                    anchor_translation.x = margin.left;
                }
                else if ((anchor_flags & gui_anchor_flag_right) == gui_anchor_flag_right)
                {
                    anchor_location.x = sibling_bounds.max.x;
                    anchor_translation.x = -(absolute_size.x + margin.right);
                }
            }

            if ((anchor_flags & gui_anchor_flag_vertical) == gui_anchor_flag_vertical)
            {
                anchor_location.y = (sibling_bounds.max.y - sibling_bounds.min.y) / 2;
                anchor_translation.y = -((absolute_size.y / 2) + ((margin.top - margin.bottom) / 2));
            }
            else
            {
                if ((anchor_flags & gui_anchor_flag_bottom) == gui_anchor_flag_bottom)
                {
                    anchor_location.y = sibling_bounds.min.y;
                    anchor_translation.y = margin.bottom;
                }
                else if ((anchor_flags & gui_anchor_flag_top) == gui_anchor_flag_top)
                {
                    anchor_location.y = sibling_bounds.max.y;
                    anchor_translation.y = -(absolute_size.y + margin.top);
                }
            }

            bounds += sibling_bounds.min + anchor_location + anchor_translation + anchor_offset;

            break;
        }

        size = bounds.size();
    }

    void gui_node_t::clean()
    {
        std::function<void(boost::shared_ptr<gui_node_t>&, aabb2_t&)> clean_function = [&](boost::shared_ptr<gui_node_t>& node, aabb2_t& parent_bounds)
        {
            node->on_clean_begin();   //gives opportunity for custom resizing based on content etc.

            node->clean_internal(parent_bounds);

            auto children_bounds = node->bounds - node->padding;

            for (auto child : node->children)
            {
                clean_function(child, children_bounds);
            }

            node->is_dirty = false;

            node->on_clean_end();
        };

        auto padded_bounds = bounds - padding;

        clean_function(shared_from_this(), padded_bounds);
    }

    void gui_node_t::tick(float32_t dt)
    {
        for (auto child : children)
        {
            child->tick(dt);
        }
    }

    void gui_node_t::on_input_event(input_event_t& input_event)
    {
        if (input_event.is_consumed)
        {
            return;
        }

        for (auto& child : children)
        {
            child->on_input_event(input_event);

            if (input_event.is_consumed)
            {
                break;
            }
        }
    }

    const gui_size_mode_e gui_node_t::get_size_mode(bool is_recursive) const
    {
        if (is_recursive && size_mode == gui_size_mode_e::inherit)
        {
            if (parent)
            {
                return parent->get_size_mode(is_recursive);
            }

            return gui_size_mode_e::absolute;
        }

        return size_mode;
    }

    void gui_node_t::dirty()
    {
        if (is_dirty)
        {
            return;
        }

        is_dirty = true;
        
        if (parent)
        {
            parent->dirty();
        }
    }

    void gui_node_t::set_size(const vec2_t & size, gui_size_mode_e size_mode)
    {
        desired_size = size;
        this->size_mode = size_mode;

        dirty();
    }

    void gui_node_t::render(const mat4_t& world_matrix, const mat4_t& view_projection_matrix)
    {
        if (is_hidden)
        {
            return;
        }

        //TODO: debug rendering?

        bool should_clip = this->should_clip;    //temporary variable in case value changes during render calls somehow

        if (should_clip)
        {
            //stencil
            auto gpu_stencil_state = gpu.stencil.get_state();

            gpu_stencil_state.is_enabled = true;

            gpu_stencil_state.function.func = gpu_t::stencil_function_e::never;
            gpu_stencil_state.function.ref = 1;
            gpu_stencil_state.function.mask = 0xFF;

            gpu_stencil_state.operations.fail = gpu_t::stencil_operation_e::replace;
            gpu_stencil_state.operations.zfail = gpu_t::stencil_operation_e::keep;
            gpu_stencil_state.operations.zpass = gpu_t::stencil_operation_e::keep;

            gpu_stencil_state.mask = 0xFF;

            gpu.stencil.push_state(gpu_stencil_state);

            //color
            auto gpu_color_state = gpu.color.get_state();

            gpu_color_state.mask.r = false;
            gpu_color_state.mask.g = false;
            gpu_color_state.mask.b = false;
            gpu_color_state.mask.a = false;

            gpu.color.push_state(gpu_color_state);

            //depth
            auto gpu_depth_state = gpu.depth.get_state();

            gpu_depth_state.should_write_mask = false;

            gpu.depth.push_state(gpu_depth_state);

            gpu.clear(gpu_t::clear_flag_depth | gpu_t::clear_flag_stencil);

            render_rectangle(world_matrix, view_projection_matrix, rectangle_t(bounds), true);

            gpu.color.pop_state();
            gpu.depth.pop_state();

            gpu_stencil_state.mask = 0;
            gpu_stencil_state.function.func = gpu_t::stencil_function_e::notequal;
            gpu_stencil_state.function.ref = 0;
            gpu_stencil_state.function.mask = 0xFF;

            gpu.stencil.pop_state();
            gpu.stencil.push_state(gpu_stencil_state);
        }

        on_render(world_matrix, view_projection_matrix);

        if (should_clip)
        {
            gpu.stencil.pop_state();
        }
    }

    void gui_node_t::on_render(const mat4_t& world_matrix, const mat4_t& view_projection_matrix)
    {
        on_render_begin(world_matrix, view_projection_matrix);

        for (auto& child : children)
        {
            child->render(world_matrix, view_projection_matrix);
        }

        on_render_end(world_matrix, view_projection_matrix);
    }

    void gui_node_t::on_render_begin(const mat4_t& world_matrix, const mat4_t& view_projection_matrix)
    {
#if defined(DEBUG)
        render_rectangle(world_matrix, view_projection_matrix, rectangle_t(get_bounds()));
#endif
    }
}
