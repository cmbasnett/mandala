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

        //TODO: might be able to forego dirtying parent if removing this node doesn't
        //affect positioning of sibling elements or sizing of parent element
        //if (dock_mode != gui_dock_mode_e::NONE)

        //mark parent as dirty
        parent->dirty();

        //unparent
        parent = nullptr;
        
        root = shared_from_this();

        //mark as dirty
        dirty();
    }

    void gui_node_t::adopt(const boost::shared_ptr<gui_node_t>& node)
    {
        if (node == nullptr)
        {
            throw std::invalid_argument("node cannot be null");
        }

        if (node.get() == this)
        {
            throw std::invalid_argument("nodes cannot adopt themselves");
        }

        if (node->has_parent())
        {
            //orphan node from parent
            node->orphan();
        }

        const auto children_itr = std::find(children.begin(), children.end(), node);

        if (children_itr != children.end())
        {
            throw std::exception("node is already a child");
        }

        //add node to child list
        children.push_back(node);

        //set node's new parent
        node->parent = shared_from_this();

        //set node's new root
        node->root = get_root();

        //mark as dirty
        dirty();
    }

    const boost::shared_ptr<gui_node_t>& gui_node_t::get_root() const
    {
        if (parent)
        {
            return parent->get_root();
        }
    }

    void gui_node_t::clean()
    {
        std::function<void(boost::shared_ptr<gui_node_t>&, aabb2_t&)> clean_node = [&clean_node](boost::shared_ptr<gui_node_t>& node, aabb2_t& sibling_bounds)
        {
            node->on_clean_begin();

            auto absolute_desired_size = node->desired_size;

            if (node->get_size_modes_real().get_x() == gui_size_mode_e::RELATIVE)
            {
                if (node->has_parent())
                {
                    absolute_desired_size.x *= node->parent->bounds.size().x;
                }
                else
                {
                    absolute_desired_size.x *= sibling_bounds.size().x;
                }
            }

            if (node->get_size_modes_real().get_y() == gui_size_mode_e::RELATIVE)
            {
                if (node->has_parent())
                {
                    absolute_desired_size.y *= node->parent->bounds.size().y;
                }
                else
                {
                    absolute_desired_size.y *= sibling_bounds.size().y;
                }
            }

            if (node->get_size_modes_real().get_x() == gui_size_mode_e::AXIS_SCALE)
            {
                switch (node->dock_mode)
                {
                case gui_dock_mode_e::LEFT:
                case gui_dock_mode_e::RIGHT:
                    absolute_desired_size.x = sibling_bounds.height() * node->desired_size.x;
                    break;
                case gui_dock_mode_e::NONE:
                    absolute_desired_size.x *= absolute_desired_size.y;
                    break;
                default:
                    break;
                }
            }
            else if (node->get_size_modes_real().get_y() == gui_size_mode_e::AXIS_SCALE)
            {
                switch (node->dock_mode)
                {
                case gui_dock_mode_e::BOTTOM:
                case gui_dock_mode_e::TOP:
                    absolute_desired_size.y = sibling_bounds.width() * node->desired_size.y;
                    break;
                case gui_dock_mode_e::NONE:
                    absolute_desired_size.y *= absolute_desired_size.x;
                    break;
                default:
                    break;
                }
            }

            switch (node->dock_mode)
            {
            case gui_dock_mode_e::BOTTOM:
                node->bounds.min = sibling_bounds.min;
                node->bounds.max.x = sibling_bounds.max.x;
                node->bounds.max.y = sibling_bounds.min.y + absolute_desired_size.y + node->padding.vertical();

                node->bounds -= node->margin;

                sibling_bounds.min.y += node->bounds.height() + node->margin.vertical();

                break;
            case gui_dock_mode_e::FILL:
                node->bounds = sibling_bounds - node->margin;

                break;
            case gui_dock_mode_e::LEFT:
                node->bounds.min = sibling_bounds.min;
                node->bounds.max.x = sibling_bounds.min.x + absolute_desired_size.x + node->padding.horizontal();
                node->bounds.max.y = sibling_bounds.max.y;

                node->bounds -= node->margin;

                sibling_bounds.min.x += node->bounds.width() + node->margin.horizontal();

                break;
            case gui_dock_mode_e::RIGHT:
                node->bounds.min.x = sibling_bounds.max.x - absolute_desired_size.x - node->padding.horizontal();
                node->bounds.min.y = sibling_bounds.min.y;
                node->bounds.max = sibling_bounds.max;

                node->bounds -= node->margin;

                sibling_bounds.max.x -= node->bounds.width() + node->margin.horizontal();

                break;
            case gui_dock_mode_e::TOP:
                node->bounds.min.x = sibling_bounds.min.x;
                node->bounds.min.y = sibling_bounds.max.y - absolute_desired_size.y - node->padding.vertical();
                node->bounds.max = sibling_bounds.max;

                node->bounds -= node->margin;

                sibling_bounds.max.y -= node->bounds.height() + node->margin.vertical();

                break;
            default:
                node->bounds.min = vec2_t(0);
                node->bounds.max = absolute_desired_size;

                vec2_t anchor_location;
                vec2_t anchor_translation;

                if ((node->anchor_flags & GUI_ANCHOR_FLAG_HORIZONTAL) == GUI_ANCHOR_FLAG_HORIZONTAL)
                {
                    anchor_location.x = (sibling_bounds.max.x - sibling_bounds.min.x) / 2;
                    anchor_translation.x = -((absolute_desired_size.x / 2) + ((node->margin.right - node->margin.left) / 2));
                }
                else
                {
                    if ((node->anchor_flags & GUI_ANCHOR_FLAG_LEFT) == GUI_ANCHOR_FLAG_LEFT)
                    {
                        anchor_location.x = sibling_bounds.min.x;
                        anchor_translation.x = node->margin.left;
                    }
                    else if ((node->anchor_flags & GUI_ANCHOR_FLAG_RIGHT) == GUI_ANCHOR_FLAG_RIGHT)
                    {
                        anchor_location.x = sibling_bounds.max.x;
                        anchor_translation.x = -(absolute_desired_size.x + node->margin.right);
                    }
                }

                if ((node->anchor_flags & GUI_ANCHOR_FLAG_VERTICAL) == GUI_ANCHOR_FLAG_VERTICAL)
                {
                    anchor_location.y = (sibling_bounds.max.y - sibling_bounds.min.y) / 2;
                    anchor_translation.y = -((absolute_desired_size.y / 2) + ((node->margin.top - node->margin.bottom) / 2));
                }
                else
                {
                    if ((node->anchor_flags & GUI_ANCHOR_FLAG_BOTTOM) == GUI_ANCHOR_FLAG_BOTTOM)
                    {
                        anchor_location.y = sibling_bounds.min.y;
                        anchor_translation.y = node->margin.bottom;
                    }
                    else if ((node->anchor_flags & GUI_ANCHOR_FLAG_TOP) == GUI_ANCHOR_FLAG_TOP)
                    {
                        anchor_location.y = sibling_bounds.max.y;
                        anchor_translation.y = -(absolute_desired_size.y + node->margin.top);
                    }
                }

                node->bounds += anchor_location + anchor_translation + node->anchor_offset;

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

        //HACK: something is amiss with this way of doing things.
        //margins can be accumulated with every tick unless we
        //pad the bounds passed into clean_node. this seems incorrect,
        //but will work for now.
        auto padded_bounds = bounds + margin;

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

    bool gui_node_t::on_input_event(input_event_t& input_event)
    {
        for (auto children_itr = children.begin(); children_itr != children.end(); ++children_itr)
        {
            if ((*children_itr)->on_input_event(input_event))
            {
                return true;
            }
        }

        return false;
    }

    const gui_size_modes_t gui_node_t::get_size_modes_real() const
    {
        auto real_size_modes = size_modes;

        if (real_size_modes.get_x() == gui_size_mode_e::INHERIT)
        {
            if (parent)
            {
                real_size_modes.set_x(parent->get_size_modes_real().get_x());
            }
            else
            {
                real_size_modes.set_x(gui_size_mode_e::ABSOLUTE);
            }
        }

        if (real_size_modes.get_y() == gui_size_mode_e::INHERIT)
        {
            if (parent)
            {
                real_size_modes.set_y(parent->get_size_modes_real().get_y());
            }
            else
            {
                real_size_modes.set_y(gui_size_mode_e::ABSOLUTE);
            }
        }

        return real_size_modes;
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

            gpu_stencil_state.function.func = gpu_t::stencil_function_e::NEVER;
            gpu_stencil_state.function.ref = 1;
            gpu_stencil_state.function.mask = 0xFF;

            gpu_stencil_state.operations.fail = gpu_t::stencil_operation_e::REPLACE;
            gpu_stencil_state.operations.zfail = gpu_t::stencil_operation_e::KEEP;
            gpu_stencil_state.operations.zpass = gpu_t::stencil_operation_e::KEEP;

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

            gpu.clear(gpu_t::CLEAR_FLAG_DEPTH | gpu_t::CLEAR_FLAG_STENCIL);

            render_rectangle(world_matrix, view_projection_matrix, rectangle_t(bounds), true);

            gpu.color.pop_state();
            gpu.depth.pop_state();

            gpu_stencil_state.mask = 0;
            gpu_stencil_state.function.func = gpu_t::stencil_function_e::NOTEQUAL;
            gpu_stencil_state.function.ref = 0;
            gpu_stencil_state.function.mask = 0xFF;

            gpu.stencil.pop_state();
            gpu.stencil.push_state(gpu_stencil_state);
        }

        //TODO: configure this to be enable-able in-game
#if defined(DEBUG)
        render_rectangle(world_matrix, view_projection_matrix, rectangle_t(bounds));
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
