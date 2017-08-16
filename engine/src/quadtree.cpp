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

	std::vector<const quadtree::node*> quadtree::trace(const line3& ray) const
	{
		// We will store calculate the distance along the ray when it is determined to be "hit", and store it alongside the node for sorting efficiency.
		struct TraceNode
		{
			const quadtree::node* node = nullptr;
			f32 distance = 0.0f;
		};
		std::vector<TraceNode> trace_nodes;
		std::function<void(const quadtree::node*)> trace_node_recursive = [&](const quadtree::node* node)
		{
			if (node == nullptr)
			{
				return;
			}

			if (intersects(ray, node->get_bounds()) != intersect_type_e::DISJOINT)
			{
				if (node->is_leaf())
				{
					TraceNode trace_node;
					trace_node.node = node;
					trace_node.distance = glm::dot(node->get_bounds().center(), ray.end - ray.start);
					trace_nodes.push_back(trace_node);
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
		trace_node_recursive(this->get_root());
		// Sort the trace nodes by distance along the ray (closest first) [VERIFY CORRECTNESS]
		std::sort(trace_nodes.begin(), trace_nodes.end(), [&](const TraceNode& lhs, const TraceNode& rhs) {
			return lhs.distance < rhs.distance;
		});
		std::vector<const quadtree::node*> nodes;
		std::transform(trace_nodes.begin(), trace_nodes.end(), std::back_inserter(nodes), [](const TraceNode& trace_node) {
			return trace_node.node;
		});
		return nodes;
	}
}