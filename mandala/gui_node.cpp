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

namespace mandala
{
    void gui_node_t::orphan()
	{
		if (_parent.get() == nullptr)
		{
            //cannot orphan a node with no parent
			return;
		}

        auto parent_children_itr = std::find(_parent->_children.begin(), _parent->_children.end(), _parent);

        if (parent_children_itr == _parent->_children.end())
        {
            //parent does not have this node as a child, fatal error
            throw std::exception();
        }

        //remove node from parent child list
		_parent->_children.erase(parent_children_itr);

		//TODO: might be able to forego dirtying parent removing this node doesn't affect positioning of sibling elements
        //mark parent as dirty
		_parent->dirty();

        //unparent
		_parent = nullptr;

        //mark as dirty
		_is_dirty = true;
	}

    void gui_node_t::adopt(std::shared_ptr<gui_node_t> node)
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

		const auto children_itr = std::find(_children.begin(), _children.end(), node);

		if (children_itr != _children.end())
		{
			//node is already a child
			throw std::exception();
		}

        //add node to child list
		_children.push_back(node);

        //mark as dirty
		_is_dirty = true;
	}

    void gui_node_t::clean()
	{
        if (has_children())
        {
			auto children_bounds = _bounds;

			children_bounds -= _padding;

            for (auto child : _children)
            {
                switch (child->_dock_mode)
                {
				case gui_dock_mode_e::bottom:
                    child->_bounds.min = children_bounds.min;

                    child->_bounds.max.x = children_bounds.max.x;
                    child->_bounds.max.y = children_bounds.min.y + child->_size.y + child->_padding.vertical();

					child->_bounds -= child->_margin;

					children_bounds.min.y += child->_bounds.height() + child->_margin.vertical();

                    break;
                case gui_dock_mode_e::fill:
                    child->_bounds = children_bounds;

                    child->_bounds.min.x += child->_margin.left;
                    child->_bounds.min.y += child->_margin.bottom;
                    child->_bounds.max.x -= child->_margin.right;
                    child->_bounds.max.y -= child->_margin.top;

                    break;
				case gui_dock_mode_e::left:
                    child->_bounds.min = children_bounds.min;

					child->_bounds.max.x = children_bounds.min.x + child->_size.x + child->_padding.horizontal();
                    child->_bounds.max.y = children_bounds.max.y;

					child->_bounds -= child->_margin;

					children_bounds.min.x += child->_bounds.width() + child->_margin.horizontal();

                    break;
				case gui_dock_mode_e::right:
                    child->_bounds.min.x = children_bounds.max.x - child->_size.x - child->_padding.horizontal();
                    child->_bounds.min.y = children_bounds.min.y;

					child->_bounds.max = children_bounds.max;

					child->_bounds -= child->_margin;

					children_bounds.max.x -= child->_bounds.width() + child->_margin.horizontal();

                    break;
				case gui_dock_mode_e::top:
                    child->_bounds.min.x = children_bounds.min.x;
                    child->_bounds.min.y = children_bounds.max.y - child->_size.y - child->_padding.vertical();

					child->_bounds.max = children_bounds.max;

					child->_bounds -= child->_margin;

					children_bounds.max.y -= child->_bounds.height() + child->_margin.vertical();

                    break;
                default:
					child->_bounds.min = vec2_t(0);
					child->_bounds.max = child->_size;

                    vec2_t anchor_location;
                    vec2_t anchor_translation;

					if ((child->_anchor_flags & gui_anchor_flag_horizontal) == gui_anchor_flag_horizontal)
                    {
						anchor_location.x = (children_bounds.max.x - children_bounds.min.x) / 2;
						anchor_translation.x = -((child->_size.x / 2) + ((child->_margin.right - child->_margin.left) / 2));
                    }
                    else
                    {
						if ((child->_anchor_flags & gui_anchor_flag_left) == gui_anchor_flag_left)
                        {
                            anchor_location.x = children_bounds.min.x;
							anchor_translation.x = child->_margin.left;
                        }
						else if ((child->_anchor_flags & gui_anchor_flag_right) == gui_anchor_flag_right)
                        {
                            anchor_location.x = children_bounds.max.x;
							anchor_translation.x = -(child->_size.x + child->_margin.right);
                        }
                    }

					if ((child->_anchor_flags & gui_anchor_flag_vertical) == gui_anchor_flag_vertical)
                    {
						anchor_location.y = (children_bounds.max.y - children_bounds.min.y) / 2;
						anchor_translation.y = -((child->_size.y / 2) + ((child->_margin.top - child->_margin.bottom) / 2));
                    }
                    else
                    {
						if ((child->_anchor_flags & gui_anchor_flag_bottom) == gui_anchor_flag_bottom)
                        {
                            anchor_location.y = children_bounds.min.y;
							anchor_translation.y = child->_margin.bottom;
                        }
						else if ((child->_anchor_flags & gui_anchor_flag_top) == gui_anchor_flag_top)
                        {
                            anchor_location.y = children_bounds.max.y;
							anchor_translation.y = -(child->_size.y + child->_margin.top);
                        }
                    }

					child->_bounds += anchor_location + anchor_translation + child->_anchor_offset;

                    break;
                }

				child->clean();

				child->_size = child->_bounds.size();
            }
        }

		_is_dirty = false;
	}

    void gui_node_t::tick(float32_t dt)
    {
        for (auto child : _children)
        {
            child->tick(dt);
        }
    }

    void gui_node_t::on_input_event(input_event_t& input_event)
	{
        if (input_event.is_consumed)
        {
            //input event already consumed
            return;
        }

        for (auto& child : _children)
        {
            child->on_input_event(input_event);

            if (input_event.is_consumed)
            {
                //input event consumed by child
                break;
            }
        }
	}

	bool gui_node_t::is_dirty() const
	{
		//TODO: this is naive and stupid, change eventually to have dirty status cascade to ancestors
		if (_is_dirty)
		{
			return true;
		}

		for (auto& child : _children)
		{
			if (child->is_dirty())
			{
				return true;
			}
		}

		return false;
	}

    bool gui_node_t::trace(std::shared_ptr<gui_node_t> node, gui_node_t::trace_args_t args, gui_node_t::trace_result_t& result)
	{
		if (intersects(args.circle, node->_bounds) != intersect_type_e::intersect)
		{
			return false;
		}

		result.nodes_hit.push_back(node);

        std::vector<std::shared_ptr<gui_node_t>> _children_hit;

		for (auto& child : node->_children)
		{
			if (trace(child, args, result))
			{
				_children_hit.push_back(child);

				result.nodes_hit.push_back(child);
			}
		}

		return true;
	}

    void gui_node_t::render(mat4_t world_matrix, mat4_t view_projection_matrix)
    {
        if (is_hidden())
        {
            return;
        }

        //TODO: debug rendering?

        bool should_clip = _should_clip;    //temporary variable in case value changes during render calls somehow

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

            gpu.clear(gpu_t::clear_flag_stencil);

            //TODO: draw bounds to stencil buffer
        }

        render_override(world_matrix, view_projection_matrix);

        for (auto& child : _children)
        {
            child->render(world_matrix, view_projection_matrix);
        }

        if (should_clip)
        {
            gpu.depth.pop_state();
            gpu.color.pop_state();
            gpu.stencil.pop_state();
        }
    }

    void gui_node_t::render_override(mat4_t world_matrix, mat4_t view_projection_matrix)
    {
    }
}
