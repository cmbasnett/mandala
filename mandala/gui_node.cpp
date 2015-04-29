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

    void gui_node_t::clean()
    {
        std::function<void(boost::shared_ptr<gui_node_t>&, aabb2_t&)> clean_node = [&clean_node](boost::shared_ptr<gui_node_t>& node, aabb2_t& sibling_bounds)
        {
            node->on_clean_begin();

            auto absolute_size = node->desired_size;

            switch (node->get_size_mode())
            {
            case gui_size_mode_e::relative:
                if (node->has_parent())   //TODO: doesn't work yet; parents not being set for some reason
                {
                    absolute_size *= node->parent->bounds.size();
                }
                else
                {
                    absolute_size *= sibling_bounds.size();
                }

                break;
            }

            switch (node->dock_mode)
            {
            case gui_dock_mode_e::bottom:
                node->bounds.min = sibling_bounds.min;
                node->bounds.max.x = sibling_bounds.max.x;
                node->bounds.max.y = sibling_bounds.min.y + absolute_size.y + node->padding.vertical();

                node->bounds -= node->margin;

                sibling_bounds.min.y += node->bounds.height() + node->margin.vertical();

                break;
            case gui_dock_mode_e::fill:
                node->bounds = sibling_bounds - node->margin;

                break;
            case gui_dock_mode_e::left:
                node->bounds.min = sibling_bounds.min;
                node->bounds.max.x = sibling_bounds.min.x + absolute_size.x + node->padding.horizontal();
                node->bounds.max.y = sibling_bounds.max.y;

                node->bounds -= node->margin;

                sibling_bounds.min.x += node->bounds.width() + node->margin.horizontal();

                break;
            case gui_dock_mode_e::right:
                node->bounds.min.x = sibling_bounds.max.x - absolute_size.x - node->padding.horizontal();
                node->bounds.min.y = sibling_bounds.min.y;
                node->bounds.max = sibling_bounds.max;

                node->bounds -= node->margin;

                sibling_bounds.max.x -= node->bounds.width() + node->margin.horizontal();

                break;
            case gui_dock_mode_e::top:
                node->bounds.min.x = sibling_bounds.min.x;
                node->bounds.min.y = sibling_bounds.max.y - absolute_size.y - node->padding.vertical();
                node->bounds.max = sibling_bounds.max;

                node->bounds -= node->margin;

                sibling_bounds.max.y -= node->bounds.height() + node->margin.vertical();

                break;
            default:
                node->bounds.min = vec2_t(0);
                node->bounds.max = absolute_size;

                vec2_t anchor_location;
                vec2_t anchor_translation;

                if ((node->anchor_flags & gui_anchor_flag_horizontal) == gui_anchor_flag_horizontal)
                {
                    anchor_location.x = (sibling_bounds.max.x - sibling_bounds.min.x) / 2;
                    anchor_translation.x = -((absolute_size.x / 2) + ((node->margin.right - node->margin.left) / 2));
                }
                else
                {
                    if ((node->anchor_flags & gui_anchor_flag_left) == gui_anchor_flag_left)
                    {
                        anchor_location.x = sibling_bounds.min.x;
                        anchor_translation.x = node->margin.left;
                    }
                    else if ((node->anchor_flags & gui_anchor_flag_right) == gui_anchor_flag_right)
                    {
                        anchor_location.x = sibling_bounds.max.x;
                        anchor_translation.x = -(absolute_size.x + node->margin.right);
                    }
                }

                if ((node->anchor_flags & gui_anchor_flag_vertical) == gui_anchor_flag_vertical)
                {
                    anchor_location.y = (sibling_bounds.max.y - sibling_bounds.min.y) / 2;
                    anchor_translation.y = -((absolute_size.y / 2) + ((node->margin.top - node->margin.bottom) / 2));
                }
                else
                {
                    if ((node->anchor_flags & gui_anchor_flag_bottom) == gui_anchor_flag_bottom)
                    {
                        anchor_location.y = sibling_bounds.min.y;
                        anchor_translation.y = node->margin.bottom;
                    }
                    else if ((node->anchor_flags & gui_anchor_flag_top) == gui_anchor_flag_top)
                    {
                        anchor_location.y = sibling_bounds.max.y;
                        anchor_translation.y = -(absolute_size.y + node->margin.top);
                    }
                }

                node->bounds += sibling_bounds.min + anchor_location + anchor_translation + node->anchor_offset;

                break;
            }

            node->size = node->bounds.size();

            auto children_bounds = node->bounds - node->padding;

            for (auto child : node->children)
            {
                clean_node(child, children_bounds);
            }

            node->is_dirty = false;

            node->on_clean_end();
        };

        auto padded_bounds = bounds - padding;

        clean_node(shared_from_this(), padded_bounds);
    }

    void gui_node_t::tick(float32_t dt)
    {
        on_tick_begin(dt);

        for (auto child : children)
        {
            child->tick(dt);
        }

        on_tick_end(dt);
    }

    void gui_node_t::on_input_event(input_event_t& input_event)
    {
        for (auto children_itr = children.begin(); !input_event.is_consumed && children_itr != children.end(); ++children_itr)
        {
            (*children_itr)->on_input_event(input_event);
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
        
        if (parent) //TODO: this won't work until parents are correctly set
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

    void gui_node_t::render(mat4_t world_matrix, mat4_t view_projection_matrix)
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

#if defined(DEBUG)
        //render_rectangle(world_matrix, view_projection_matrix, rectangle_t(bounds));
#endif

        on_render_begin(world_matrix, view_projection_matrix);

        for (auto& child : children)
        {
            child->render(world_matrix, view_projection_matrix);
        }

        on_render_end(world_matrix, view_projection_matrix);

        if (should_clip)
        {
            gpu.stencil.pop_state();
        }
    }
}
