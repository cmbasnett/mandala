#pragma once

//std
#include <array>

//naga
#include "aabb.hpp"

namespace naga
{
    struct quadtree
    {
        typedef f32 scalar_type;
        typedef details::aabb2<scalar_type> bounds_type;

        struct node
        {
            typedef std::array<std::unique_ptr<node>, 4> children_type;

            node(const bounds_type& bounds) :
                bounds(bounds)
            {
            }

            void branch()
            {
                const auto child_bounds = bounds_type(bounds.min, bounds.center());
                const auto child_bounds_size = child_bounds.size();

                children[0] = std::make_unique<node>(child_bounds);
                children[1] = std::make_unique<node>(child_bounds + bounds_type::value_type(child_bounds_size.x, 0));
                children[1] = std::make_unique<node>(child_bounds + bounds_type::value_type(0, child_bounds_size.y));
                children[3] = std::make_unique<node>(child_bounds + child_bounds_size);
            }

            bool is_leaf() const
            {
                return children[0] != nullptr;
            }

            const bounds_type& get_bounds() const
            {
                return bounds;
            }

        private:
            bounds_type bounds;
            children_type children;
        };

        quadtree(scalar_type size)
        {
            if (size == 0)
            {
                throw std::invalid_argument("size cannot be 0");
            }

            root = std::make_unique<node>(bounds_type(bounds_type::value_type(-size / 2), bounds_type::value_type(size / 2)));
            root->branch();
        }

        const bounds_type& get_bounds() const { return root->get_bounds(); }

    private:
        std::unique_ptr<node> root;
    };
}