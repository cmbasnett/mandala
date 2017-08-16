#pragma once

//std
#include <array>

//naga
#include "aabb.hpp"
#include "line.hpp"

namespace naga
{
    struct quadtree
    {
        typedef f32 scalar_type;
        typedef details::aabb3<scalar_type> bounds_type;

        struct node
		{
			typedef std::array<node*, 4> children_type;

			node(const bounds_type& bounds) :
				bounds(bounds)
			{
				children.fill(nullptr);
			}

            void branch(size_t iterations);

            bool is_leaf() const { return children[0] == nullptr; }
            const bounds_type& get_bounds() const { return bounds; }
			const children_type& get_children() const { return children; }

        private:
            bounds_type bounds;
			children_type children;
        };

        quadtree(scalar_type size, scalar_type height, scalar_type leaf_size);

        const bounds_type& get_bounds() const { return root->get_bounds(); }
		const node* get_root() const { return root; }

		std::vector<const node*> trace(const line3& ray) const;

    private:
		node* root = nullptr;
    };
}