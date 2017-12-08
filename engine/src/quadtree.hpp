#pragma once

//std
#include <array>

//naga
#include "aabb.hpp"
#include "line.hpp"

namespace naga
{
    struct QuadTree
    {
        typedef f32 ScalarType;
		typedef details::AABB3<ScalarType> BoundsType;

        struct Node
		{
			typedef std::array<Node*, 4> ChildrenType;

			Node(const BoundsType& bounds) :
				bounds(bounds)
			{
				children.fill(nullptr);
			}

            void branch(size_t iterations);

            bool is_leaf() const { return children[0] == nullptr; }
			const BoundsType& get_bounds() const { return bounds; }
			const ChildrenType& get_children() const { return children; }

        private:
			BoundsType bounds;
			ChildrenType children;
        };

		QuadTree(ScalarType size, ScalarType height, ScalarType leaf_size);

		const BoundsType& get_bounds() const { return root->get_bounds(); }
		const Node* get_root() const { return root; }

		std::vector<const Node*> trace(const Line3& ray) const;

    private:
		Node* root = nullptr;
    };
}