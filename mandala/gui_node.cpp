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

    void gui_node_t::adopt(const std::shared_ptr<gui_node_t>& node)
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

    void gui_node_t::clean()
    {
        if (has_children())
        {
            const auto padded_bounds = bounds - padding;
            auto children_bounds = padded_bounds;

            for (auto child : children)
            {
                auto child_size = child->desired_size;
                const auto child_size_mode = child->get_size_mode(true);

                switch (child_size_mode)
                {
                case gui_size_mode_e::relative:
                    child_size = padded_bounds.size() * child->desired_size;
                    break;
                }

                switch (child->dock_mode)
                {
                case gui_dock_mode_e::bottom:
                    child->bounds.min = children_bounds.min;

                    child->bounds.max.x = children_bounds.max.x;
                    child->bounds.max.y = children_bounds.min.y + child_size.y + child->padding.vertical();

                    child->bounds -= child->margin;

                    children_bounds.min.y += child->bounds.height() + child->margin.vertical();

                    break;
                case gui_dock_mode_e::fill:
                    child->bounds = children_bounds;

                    child->bounds.min.x += child->margin.left;
                    child->bounds.min.y += child->margin.bottom;
                    child->bounds.max.x -= child->margin.right;
                    child->bounds.max.y -= child->margin.top;

                    break;
                case gui_dock_mode_e::left:
                    child->bounds.min = children_bounds.min;

                    child->bounds.max.x = children_bounds.min.x + child_size.x + child->padding.horizontal();
                    child->bounds.max.y = children_bounds.max.y;

                    child->bounds -= child->margin;

                    children_bounds.min.x += child->bounds.width() + child->margin.horizontal();

                    break;
                case gui_dock_mode_e::right:
                    child->bounds.min.x = children_bounds.max.x - child_size.x - child->padding.horizontal();
                    child->bounds.min.y = children_bounds.min.y;

                    child->bounds.max = children_bounds.max;

                    child->bounds -= child->margin;

                    children_bounds.max.x -= child->bounds.width() + child->margin.horizontal();

                    break;
                case gui_dock_mode_e::top:
                    child->bounds.min.x = children_bounds.min.x;
                    child->bounds.min.y = children_bounds.max.y - child_size.y - child->padding.vertical();

                    child->bounds.max = children_bounds.max;

                    child->bounds -= child->margin;

                    children_bounds.max.y -= child->bounds.height() + child->margin.vertical();

                    break;
                default:
                    child->bounds.min = vec2_t(0);
                    child->bounds.max = child_size;

                    vec2_t anchor_location;
                    vec2_t anchor_translation;

                    if ((child->anchor_flags & gui_anchor_flag_horizontal) == gui_anchor_flag_horizontal)
                    {
                        anchor_location.x = (children_bounds.max.x - children_bounds.min.x) / 2;
                        anchor_translation.x = -((child_size.x / 2) + ((child->margin.right - child->margin.left) / 2));
                    }
                    else
                    {
                        if ((child->anchor_flags & gui_anchor_flag_left) == gui_anchor_flag_left)
                        {
                            anchor_location.x = children_bounds.min.x;
                            anchor_translation.x = child->margin.left;
                        }
                        else if ((child->anchor_flags & gui_anchor_flag_right) == gui_anchor_flag_right)
                        {
                            anchor_location.x = children_bounds.max.x;
                            anchor_translation.x = -(child_size.x + child->margin.right);
                        }
                    }

                    if ((child->anchor_flags & gui_anchor_flag_vertical) == gui_anchor_flag_vertical)
                    {
                        anchor_location.y = (children_bounds.max.y - children_bounds.min.y) / 2;
                        anchor_translation.y = -((child_size.y / 2) + ((child->margin.top - child->margin.bottom) / 2));
                    }
                    else
                    {
                        if ((child->anchor_flags & gui_anchor_flag_bottom) == gui_anchor_flag_bottom)
                        {
                            anchor_location.y = children_bounds.min.y;
                            anchor_translation.y = child->margin.bottom;
                        }
                        else if ((child->anchor_flags & gui_anchor_flag_top) == gui_anchor_flag_top)
                        {
                            anchor_location.y = children_bounds.max.y;
                            anchor_translation.y = -(child_size.y + child->margin.top);
                        }
                    }

                    child->bounds += children_bounds.min + anchor_location + anchor_translation + child->anchor_offset;

                    break;
                }

                child->clean();

                child->size = child->bounds.size();

                child->on_cleaned();
            }
        }

        is_dirty = false;
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

    const gui_size_mode_e gui_node_t::get_size_mode(bool is_recursive = true) const
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
            //TODO: cascade dirtyiness upwards if parent relies on this element for sizing, placement etc.
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
