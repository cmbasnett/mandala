//naga
#include "quadtree.hpp"

#include <boost/make_shared.hpp>

namespace naga
{
    quadtree::quadtree(size_t size, size_t leaf_size)
    {
        if (size == 0)
        {
            throw std::invalid_argument("size cannot be 0");
        }

        if (leaf_size == 0)
        {
            throw std::invalid_argument("leaf size cannot be 0");
        }

        if (size % leaf_size != 0)
        {
            throw std::invalid_argument("leaf size must be a multiple of size");
        }

        auto half_size = static_cast<f32>(size) / 2;

        auto bounds = bounds_type(
            bounds_type::value_type(-half_size, 0, -half_size),
            bounds_type::value_type(half_size, 0, half_size));

        auto iterations = 0;

        while (size != leaf_size)
        {
            size /= 2;

            ++iterations;
        }

        root = std::make_shared<node>(bounds);
        root->branch(iterations);
    }

    void quadtree::node::branch(size_t iterations)
    {
        if (iterations <= 0)
        {
            return;
        }

        const auto child_bounds = bounds_type(bounds.min, bounds.center());
        const auto child_bounds_size = child_bounds.size();

        children[0] = boost::make_shared<node>(child_bounds);
        children[1] = boost::make_shared<node>(child_bounds + bounds_type::value_type(child_bounds_size.x, 0, 0));
        children[2] = boost::make_shared<node>(child_bounds + bounds_type::value_type(0, 0, child_bounds_size.z));
        children[3] = boost::make_shared<node>(child_bounds + child_bounds_size);

        if (--iterations > 0)
        {
            for (auto& child : children)
            {
                child->branch(iterations);
            }
        }
    }
}