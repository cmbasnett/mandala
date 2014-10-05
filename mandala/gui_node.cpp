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

namespace mandala
{
    void gui_node_t::orphan()
	{
		if (_parent.get() == nullptr)
		{
			return;
		}

		auto parent_children_itr = std::find(_parent->_children.begin(), _parent->_children.end(), _parent);

		if (parent_children_itr == _parent->_children.end())
		{
			throw std::exception();
		}
			
		_parent->_children.erase(parent_children_itr);

		_parent->_is_dirty = true;

		_parent = nullptr;

		_is_dirty = true;
	}

    void gui_node_t::adopt(std::shared_ptr<gui_node_t> node)
	{
		if (node == nullptr || node.get() == this || node->has_parent())
        {
            throw std::exception();
        }

		auto children_itr = std::find(_children.begin(), _children.end(), node);

		if (children_itr != _children.end())
		{
			//node already exists in _children
			throw std::exception();
		}

		_children.push_back(node);

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

    void gui_node_t::on_input_event(input_event_t& input_event)
	{
        for (auto& child : _children)
        {
            child->on_input_event(input_event);

            if (input_event.is_consumed)
            {
                break;
            }
        }
	}

	bool gui_node_t::is_dirty() const
	{
		//TODO: this is naive and stupid, change eventually
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

        for (auto& child : _children)
        {
            child->render(world_matrix, view_projection_matrix);
        }
    }
}
