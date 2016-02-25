#pragma once

//std
#include <array>

//boost
#include <boost\shared_ptr.hpp>

//naga
#include "aabb.hpp"

namespace naga
{
    struct quadtree
    {
        typedef f32 scalar_type;
        typedef details::aabb3<scalar_type> bounds_type;

        struct node
        {
			typedef std::array<boost::shared_ptr<node>, 4> children_type;

			node(const bounds_type& bounds) :
                bounds(bounds)
            {
            }

            void branch(size_t iterations);

            bool is_leaf() const { return children[0] != nullptr; }
            const bounds_type& get_bounds() const { return bounds; }
			const children_type& get_children() const { return children; }

        private:
            bounds_type bounds;
            children_type children;
        };

        quadtree(size_t size, size_t leaf_size);

        const bounds_type& get_bounds() const { return root->get_bounds(); }
		std::weak_ptr<node> get_root() const { return root; }

    private:
        std::shared_ptr<node> root;
    };
}