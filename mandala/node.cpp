#pragma once

//mandala
#include "node.h"

namespace mandala
{
	namespace gui
	{
		node_t::node_t() :
            parent(nullptr),
            dock_mode(dock_mode_t::NONE),
            anchor_flags(ANCHOR_FLAG_TOP | ANCHOR_FLAG_LEFT)
		{
		}

		node_t::~node_t()
		{
		}

		void node_t::orphan()
		{
			if (parent == nullptr)
			{
				return;
			}

			auto itr = std::find(parent->children.begin(), parent->children.end(), this);

			if (itr == parent->children.end())
			{
				throw std::exception();
			}
			
			parent->children.erase(itr);
		}

		void node_t::adopt(node_t* child)
		{
            if (child == nullptr || child == this || child->has_parent())
            {
                throw std::exception();
            }

			auto itr = std::find(children.begin(), children.end(), child);

			if (itr != children.end())
			{
				throw std::exception();
			}

			children.push_back(child);
		}

		bool node_t::clean()
		{
			bool did_clean = false;

            if (has_children())
            {
                auto children_bounds = bounds;

                //TODO: create operator function to perform this operation (e.g. aabb -/+ padding)
                children_bounds.min.x += padding.left;
                children_bounds.min.y += padding.bottom;
                children_bounds.max.x -= padding.right;
                children_bounds.max.y -= padding.top;

                for (auto child : children)
                {
                    switch (child->dock_mode)
                    {
                    case dock_mode_t::BOTTOM:
                        child->bounds.min = children_bounds.min;

                        child->bounds.max.x = children_bounds.max.x;
                        child->bounds.max.y = children_bounds.min.y + child->size.y + child->padding.vertical();

                        children_bounds.min.y += child->bounds.height();

                        break;
                    case dock_mode_t::FILL:
                        child->bounds = children_bounds;
                        break;
                    case dock_mode_t::LEFT:
                        child->bounds.min = children_bounds.min;

                        child->bounds.max.x = children_bounds.min.x + child->size.x + child->padding.horizontal();
                        child->bounds.max.y = children_bounds.max.y;

                        children_bounds.min.x += child->bounds.width();

                        break;
                    case dock_mode_t::RIGHT:
                        child->bounds.min.x = children_bounds.max.x - child->size.x - child->padding.horizontal();
                        child->bounds.min.y = children_bounds.min.y;

                        child->bounds.max = children_bounds.max;

                        children_bounds.max.x -= child->bounds.width();

                        break;
                    case dock_mode_t::TOP:
                        child->bounds.min.x = children_bounds.min.x;
                        child->bounds.min.y = children_bounds.max.y - child->size.y - child->padding.vertical();

                        child->bounds.max = children_bounds.max;

                        children_bounds.max.y -= child->bounds.height();

                        break;
                    default:
                        child->bounds.max = child->size;

                        vec2_t anchor_location;
                        vec2_t anchor_translation;

                        if ((child->anchor_flags & ANCHOR_FLAG_HORIZONTAL) == ANCHOR_FLAG_HORIZONTAL)
                        {
                            anchor_location.x = (children_bounds.max.x - children_bounds.min.x) * 0.5f;
                            anchor_translation.x = child->size.x * 0.5f;
                        }
                        else
                        {
                            if ((child->anchor_flags & ANCHOR_FLAG_LEFT) == ANCHOR_FLAG_LEFT)
                            {
                                anchor_location.x = children_bounds.min.x;
                            }
                            else if ((child->anchor_flags & ANCHOR_FLAG_RIGHT) == ANCHOR_FLAG_RIGHT)
                            {
                                anchor_location.x = children_bounds.max.x;
                                anchor_translation.x = child->size.x;
                            }
                        }

                        if ((child->anchor_flags & ANCHOR_FLAG_VERTICAL) == ANCHOR_FLAG_VERTICAL)
                        {
                            anchor_location.y = (children_bounds.max.y - children_bounds.min.y) * 0.5f;
                            anchor_translation.y = child->size.y * 0.5f;
                        }
                        else
                        {
                            if ((child->anchor_flags & ANCHOR_FLAG_BOTTOM) == ANCHOR_FLAG_BOTTOM)
                            {
                                anchor_location.y = children_bounds.min.y;
                            }
                            else if ((child->anchor_flags & ANCHOR_FLAG_TOP) == ANCHOR_FLAG_TOP)
                            {
                                anchor_location.y = children_bounds.max.y;
                                anchor_translation.y = child->size.y;
                            }
                        }

                        child->bounds += anchor_location - anchor_translation;

                        break;
                    }

                    child->size = child->bounds.size();

                    child->clean();
                }
            }

            return did_clean;
		}

        void node_t::render(mat4_t world_matrix, mat4_t view_projection_matrix)
        {
            for (auto child : children)
            {
                child->render(world_matrix, view_projection_matrix);
            }
        }
	}
}
