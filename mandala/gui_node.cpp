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
		if (parent.get() == nullptr)
		{
			return;
		}

		auto parent_children_itr = std::find(parent->children.begin(), parent->children.end(), parent);

		if (parent_children_itr == parent->children.end())
		{
			throw std::exception();
		}
			
		parent->children.erase(parent_children_itr);

		parent->is_dirty = true;

		parent = nullptr;

		is_dirty = true;
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

		is_dirty = true;
	}

    void gui_node_t::clean()
	{
        if (has_children())
        {
            auto children_bounds = bounds;

			children_bounds -= padding;

            for (auto child : children)
            {
                switch (child->dock_mode)
                {
				case gui_dock_mode_e::bottom:
                    child->bounds.min = children_bounds.min;

                    child->bounds.max.x = children_bounds.max.x;
                    child->bounds.max.y = children_bounds.min.y + child->size.y + child->padding.vertical();

					child->bounds -= child->margin;

					children_bounds.min.y += child->bounds.height() + child->margin.vertical();

                    break;
                case gui_dock_mode_e::fill:
                    child->bounds = children_bounds;
                    break;
				case gui_dock_mode_e::left:
                    child->bounds.min = children_bounds.min;

                    child->bounds.max.x = children_bounds.min.x + child->size.x + child->padding.horizontal();
                    child->bounds.max.y = children_bounds.max.y;

					child->bounds -= child->margin;

					children_bounds.min.x += child->bounds.width() + child->margin.horizontal();

                    break;
				case gui_dock_mode_e::right:
                    child->bounds.min.x = children_bounds.max.x - child->size.x - child->padding.horizontal();
                    child->bounds.min.y = children_bounds.min.y;

					child->bounds.max = children_bounds.max;

					child->bounds -= child->margin;

					children_bounds.max.x -= child->bounds.width() + child->margin.horizontal();

                    break;
				case gui_dock_mode_e::top:
                    child->bounds.min.x = children_bounds.min.x;
                    child->bounds.min.y = children_bounds.max.y - child->size.y - child->padding.vertical();

					child->bounds.max = children_bounds.max;

					child->bounds -= child->margin;

					children_bounds.max.y -= child->bounds.height() + child->margin.vertical();

                    break;
                default:
					child->bounds.min = vec2_t(0);
					child->bounds.max = child->size;

                    vec2_t anchor_location;
                    vec2_t anchor_translation;

					if ((child->anchor_flags & gui_anchor_flag_horizontal) == gui_anchor_flag_horizontal)
                    {
						anchor_location.x = (children_bounds.max.x - children_bounds.min.x) / 2;
						anchor_translation.x = -((child->size.x / 2) + ((child->margin.right - child->margin.left) / 2));
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
							anchor_translation.x = -(child->size.x + child->margin.right);
                        }
                    }

					if ((child->anchor_flags & gui_anchor_flag_vertical) == gui_anchor_flag_vertical)
                    {
						anchor_location.y = (children_bounds.max.y - children_bounds.min.y) / 2;
						anchor_translation.y = -((child->size.y / 2) + ((child->margin.top - child->margin.bottom) / 2));
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
							anchor_translation.y = -(child->size.y + child->margin.top);
                        }
                    }

					child->bounds += anchor_location + anchor_translation + child->anchor_offset;

                    break;
                }

				child->clean();

				child->size = child->bounds.size();
            }
        }

		is_dirty = false;
	}

    void gui_node_t::on_input_event(input_event_t& input_event)
	{
		input_event.is_consumed = true;
	}

    bool gui_node_t::trace(std::shared_ptr<gui_node_t> node, gui_node_t::trace_args_t args, gui_node_t::trace_result_t& result)
	{
		if (intersects(args.circle, node->bounds) != intersect_type_e::intersect)
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
		if (is_hidden)
		{
			return;
		}

        for (auto& child : children)
        {
            child->render(world_matrix, view_projection_matrix);
        }
    }
}
