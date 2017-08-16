//std
#include <vector>

//naga
#include "quadtree.hpp"
#include "collision.hpp"

// boost
#include <boost/make_shared.hpp>

namespace naga
{
	quadtree::quadtree(quadtree::scalar_type size, quadtree::scalar_type height, quadtree::scalar_type leaf_size)
    {
        if (size == 0)
        {
            throw std::invalid_argument("size cannot be 0");
        }

		if (leaf_size == 0)
		{
			throw std::invalid_argument("leaf size cannot be 0");
		}

		if (fmod(size, leaf_size) != 0.0f)
        {
            throw std::invalid_argument("leaf size must be a multiple of size");
        }

        auto half_size = static_cast<f32>(size) / 2;

        auto bounds = bounds_type(
            bounds_type::value_type(-half_size, 0, -half_size),
            bounds_type::value_type(half_size, height, half_size));

		auto iterations = static_cast<size_t>(glm::fastLog2(size) - glm::fastLog2(leaf_size));

        root = new node(bounds);
		root->branch(iterations);
    }

    void quadtree::node::branch(size_t iterations)
    {
		if (iterations <= 0)
		{
			return;
		}

		const auto child_bounds = bounds_type(bounds.min, bounds.min + bounds_type::value_type(bounds.width() / 2, bounds.height(), bounds.depth() / 2));
        const auto child_bounds_size = child_bounds.size();

        children[0] = new node(child_bounds);
        children[1] = new node(child_bounds + bounds_type::value_type(child_bounds_size.x, 0, 0));
        children[2] = new node(child_bounds + bounds_type::value_type(0, 0, child_bounds_size.z));
		children[3] = new node(child_bounds + bounds_type::value_type(child_bounds_size.x, 0, child_bounds_size.z));

        if (--iterations > 0)
        {
            for (auto& child : children)
            {
                child->branch(iterations);
            }
        }
    }

	std::vector<const quadtree::node*> quadtree::trace(const line3& ray) const {
		std::vector<const quadtree::node*> nodes;
		std::function<void(const quadtree::node*)> trace_node_recursive = [&](const quadtree::node* node)
		{
			if (node == nullptr)
			{
				return;
			}

			if (intersects(ray, node->get_bounds()) != naga::intersect_type_e::DISJOINT)
			{
				if (node->is_leaf())
				{
					nodes.push_back(node);
				}
				else
				{
					for (auto& child : node->get_children())
					{
						trace_node_recursive(child);
					}
				}
			}
		};
		trace_node_recursive(get_root());
		// TODO: sort nodes by distance along input ray?
		std::sort(nodes.begin(), nodes.end(), [&](const node* lhs, const node* rhs) {
			return false;
		});
		return nodes;
	}
}