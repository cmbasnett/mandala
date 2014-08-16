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
	void gui_node_t::set_dock_mode(gui_dock_mode_e dock_mode)
	{
		if (_dock_mode != dock_mode)
		{
			_dock_mode = dock_mode;
			_is_dirty = true;
		}
	}

	void gui_node_t::set_anchor_flags(gui_anchor_flags_type anchor_flags)
	{
		if (_anchor_flags != anchor_flags)
		{
			_anchor_flags = anchor_flags;
			_is_dirty = true;
		}
	}

	void gui_node_t::set_anchor_offset(const vec2_t& anchor_offset)
	{
		if (_anchor_offset != anchor_offset)
		{
			_anchor_offset = anchor_offset;
			_is_dirty = true;
		}
	}

	void gui_node_t::set_padding(const padding_t& padding)
	{
		if (_padding != padding)
		{
			_padding = padding;
			_is_dirty = true;
		}
	}

	void gui_node_t::set_margin(const padding_t& margin)
	{
		if (_margin != margin)
		{
			_margin = margin;
			_is_dirty = true;
		}
	}

	void gui_node_t::set_size(const size_type& size)
	{
		if (_size != size)
		{
			_size = size;
			_is_dirty = true;
		}
	}

	void gui_node_t::set_color(const vec4_t& color)
	{
		if (_color != color)
		{
			_color = color;
			_is_dirty = true;
		}
	}

	void gui_node_t::set_bounds(const bounds_type& bounds)
	{
		if (_bounds != bounds)
		{
			_bounds = bounds;
			_is_dirty = true;
		}
	}

	void gui_node_t::set_is_hidden(bool is_hidden)
	{
		if (_is_hidden != is_hidden)
		{
			_is_hidden = is_hidden;
			_is_dirty = true;
		}
	}

    void gui_node_t::orphan()
	{
		if (_parent.get() == nullptr)
		{
			return;
		}

		auto parent_children_itr = std::find(_parent->children.begin(), _parent->children.end(), _parent);

		if (parent_children_itr == _parent->children.end())
		{
			throw std::exception();
		}
			
		_parent->children.erase(parent_children_itr);

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

		auto children_itr = std::find(children.begin(), children.end(), node);

		if (children_itr != children.end())
		{
			//node already exists in children
			throw std::exception();
		}

		children.push_back(node);

		_is_dirty = true;
	}

    void gui_node_t::clean()
	{
        if (has_children())
        {
			auto children_bounds = _bounds;

			children_bounds -= _padding;

            for (auto child : children)
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
		input_event.is_consumed = true;
	}

    bool gui_node_t::trace(std::shared_ptr<gui_node_t> node, gui_node_t::trace_args_t args, gui_node_t::trace_result_t& result)
	{
		if (intersects(args.circle, node->_bounds) != intersect_type_e::intersect)
		{
			return false;
		}

		result.nodes_hit.push_back(node);

        std::vector<std::shared_ptr<gui_node_t>> children_hit;

		for (auto& child : node->children)
		{
			if (trace(child, args, result))
			{
				children_hit.push_back(child);

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

        for (auto& child : children)
        {
            child->render(world_matrix, view_projection_matrix);
        }
    }
}
