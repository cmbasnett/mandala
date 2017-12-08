//std
#include <vector>

//naga
#include "quadtree.hpp"
#include "collision.hpp"

// boost
#include <boost/make_shared.hpp>

namespace naga
{
	QuadTree::QuadTree(ScalarType size, ScalarType height, ScalarType leaf_size)
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

        auto bounds = BoundsType(
			BoundsType::VectorType(-half_size, 0, -half_size),
			BoundsType::VectorType(half_size, height, half_size));

		auto iterations = static_cast<size_t>(glm::fastLog2(size) - glm::fastLog2(leaf_size));

        root = new Node(bounds);
		root->branch(iterations);
    }

    void QuadTree::Node::branch(size_t iterations)
    {
		if (iterations <= 0)
		{
			return;
		}

		const auto child_bounds = BoundsType(bounds.min, bounds.min + BoundsType::VectorType(bounds.width() / 2, bounds.height(), bounds.depth() / 2));
        const auto child_bounds_size = child_bounds.size();

        children[0] = new Node(child_bounds);
		children[1] = new Node(child_bounds + BoundsType::VectorType(child_bounds_size.x, 0, 0));
		children[2] = new Node(child_bounds + BoundsType::VectorType(0, 0, child_bounds_size.z));
		children[3] = new Node(child_bounds + BoundsType::VectorType(child_bounds_size.x, 0, child_bounds_size.z));

        if (--iterations > 0)
        {
            for (auto& child : children)
            {
                child->branch(iterations);
            }
        }
    }

	std::vector<const QuadTree::Node*> QuadTree::trace(const Line3& ray) const
	{
		// We will store calculate the distance along the ray when it is determined to be "hit", and store it alongside the node for sorting efficiency.
		struct TraceNode
		{
			const QuadTree::Node* node = nullptr;
			f32 distance = 0.0f;
		};
		std::vector<TraceNode> trace_nodes;
		std::function<void(const QuadTree::Node*)> trace_node_recursive = [&](const QuadTree::Node* node)
		{
			if (node != nullptr && intersects(ray, node->get_bounds()) != IntersectType::DISJOINT)
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
		std::sort(trace_nodes.begin(), trace_nodes.end(), [&](const TraceNode& lhs, const TraceNode& rhs)
		{
			return lhs.distance < rhs.distance;
		});
		std::vector<const QuadTree::Node*> nodes;
		std::transform(trace_nodes.begin(), trace_nodes.end(), std::back_inserter(nodes), [](const TraceNode& trace_node)
		{
			return trace_node.node;
		});
		return nodes;
	}
}