#pragma once

//std
#include <algorithm>

//glm
#include <glm\glm.hpp>
#include <glm\ext.hpp>

//mandala
#include "../collision.hpp"
#include "../input_event.hpp"
#include "node.hpp"

namespace mandala
{
	namespace gui
	{
		void node_t::orphan()
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
		}

		void node_t::adopt(std::shared_ptr<node_t> node)
		{
			if (node == nullptr || node.get() == this || node->has_parent() || !can_have_children())
            {
                throw std::exception();
            }

			auto children_itr = std::find(children.begin(), children.end(), node);

			if (children_itr != children.end())
			{
				throw std::exception();
			}

			children.push_back(node);
		}

		bool node_t::clean()
		{
			bool did_clean = false;

            if (has_children())
            {
                auto children_bounds = bounds;

				children_bounds -= padding;

                for (auto child : children)
                {
                    switch (child->dock_mode)
                    {
                    case dock_mode_t::bottom:
                        child->bounds.min = children_bounds.min;

                        child->bounds.max.x = children_bounds.max.x;
                        child->bounds.max.y = children_bounds.min.y + child->size.y + child->padding.vertical();

						child->bounds -= child->margin;

						children_bounds.min.y += child->bounds.height() + child->margin.vertical();

                        break;
                    case dock_mode_t::fill:
                        child->bounds = children_bounds;
                        break;
                    case dock_mode_t::left:
                        child->bounds.min = children_bounds.min;

                        child->bounds.max.x = children_bounds.min.x + child->size.x + child->padding.horizontal();
                        child->bounds.max.y = children_bounds.max.y;

						child->bounds -= child->margin;

						children_bounds.min.x += child->bounds.width() + child->margin.horizontal();

                        break;
                    case dock_mode_t::right:
                        child->bounds.min.x = children_bounds.max.x - child->size.x - child->padding.horizontal();
                        child->bounds.min.y = children_bounds.min.y;

						child->bounds.max = children_bounds.max;

						child->bounds -= child->margin;

						children_bounds.max.x -= child->bounds.width() + child->margin.horizontal();

                        break;
                    case dock_mode_t::top:
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

                        if ((child->anchor_flags & anchor_flag_horizontal) == anchor_flag_horizontal)
                        {
							anchor_location.x = (children_bounds.max.x - children_bounds.min.x) / 2;
							anchor_translation.x = -((child->size.x / 2) + ((child->margin.right - child->margin.left) / 2));
                        }
                        else
                        {
                            if ((child->anchor_flags & anchor_flag_left) == anchor_flag_left)
                            {
                                anchor_location.x = children_bounds.min.x;
								anchor_translation.x = child->margin.left;
                            }
                            else if ((child->anchor_flags & anchor_flag_right) == anchor_flag_right)
                            {
                                anchor_location.x = children_bounds.max.x;
								anchor_translation.x = -(child->size.x + child->margin.right);
                            }
                        }

                        if ((child->anchor_flags & anchor_flag_vertical) == anchor_flag_vertical)
                        {
							anchor_location.y = (children_bounds.max.y - children_bounds.min.y) / 2;
							anchor_translation.y = -((child->size.y / 2) + ((child->margin.top - child->margin.bottom) / 2));
                        }
                        else
                        {
                            if ((child->anchor_flags & anchor_flag_bottom) == anchor_flag_bottom)
                            {
                                anchor_location.y = children_bounds.min.y;
								anchor_translation.y = child->margin.bottom;
                            }
                            else if ((child->anchor_flags & anchor_flag_top) == anchor_flag_top)
                            {
                                anchor_location.y = children_bounds.max.y;
								anchor_translation.y = -(child->size.y + child->margin.top);
                            }
                        }

						child->bounds += anchor_location + anchor_translation + child->anchor_offset;

                        break;
                    }

					child->size = child->bounds.size();

					child->clean();
                }
            }

            return did_clean;
		}

		void node_t::on_input_event(input_event_t& input_event)
		{
			input_event.is_consumed = true;
		}

		bool node_t::trace(std::shared_ptr<node_t> node, node_t::trace_args_t args, node_t::trace_result_t& result)
		{
			if (intersects(args.circle, node->bounds) != intersect_type_t::intersect)
			{
				return false;
			}

			result.nodes_hit.push_back(node);

			std::vector<std::shared_ptr<node_t>> children_hit;

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

        void node_t::render(mat4_t world_matrix, mat4_t view_projection_matrix)
        {
			if (is_hidden)
			{
				return;
			}

			//glBegin(GL_LINE_LOOP);
			//glVertex2f(bounds.min.x, bounds.min.y);
			//glVertex2f(bounds.min.x, bounds.max.y);
			//glVertex2f(bounds.max.x, bounds.max.y);
			//glVertex2f(bounds.max.x, bounds.min.y);
			//glEnd();

            for (auto& child : children)
            {
                child->render(world_matrix, view_projection_matrix);
            }
        }
	}
}
