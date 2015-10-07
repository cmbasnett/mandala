#pragma once

//std
#include <array>

//boost
#include <boost/make_shared.hpp>

//mandala
#include "aabb.hpp"

namespace mandala
{
    namespace details
    {
        template<typename Scalar, typename Enable = void>
        struct octree;

        template<typename Scalar>
        struct octree<Scalar, typename std::enable_if<std::is_floating_point<Scalar>::value>::type>
        {
            typedef Scalar scalar_type;
            typedef aabb3<scalar_type> bounds_type;
            typedef octree<scalar_type> type;
            typedef std::array<boost::shared_ptr<type>, 8> children_type;

            octree(scalar_type size = 2048) :
                bounds(bounds_type::value_type(-size / 2), bounds_type::value_type(size / 2))
            {
            }

            octree(const bounds_type& bounds) :
                bounds(bounds)
            {
            }

            void grow()
            {
                const auto child_bounds = bounds_type(bounds.min, bounds.center());
                const auto child_bounds_size = child_bounds.size();

                children[0] = boost::make_shared<type>(child_bounds);
                children[1] = boost::make_shared<type>(child_bounds + bounds_type::value_type(child_bounds_size.x, 0, 0));
                children[2] = boost::make_shared<type>(child_bounds + bounds_type::value_type(0, child_bounds_size.y, 0));
                children[3] = boost::make_shared<type>(child_bounds + bounds_type::value_type(child_bounds_size.x, child_bounds_size.y, 0));
                children[4] = boost::make_shared<type>(child_bounds + bounds_type::value_type(0, 0, child_bounds_size.z));
                children[5] = boost::make_shared<type>(child_bounds + bounds_type::value_type(child_bounds_size.x, 0, child_bounds_size.z));
                children[6] = boost::make_shared<type>(child_bounds + bounds_type::value_type(0, child_bounds_size.y, child_bounds_size.z));
                children[7] = boost::make_shared<type>(child_bounds + child_bounds_size);
            }

            const bounds_type& get_bounds() const { return bounds; }
            const children_type& get_children() const { return children; }
            bool is_leaf() const { return children[0] == nullptr; }

        private:
            bounds_type bounds;
            children_type children;
        };
    }

    typedef details::octree<f32> octree_f32;
    typedef details::octree<f64> octree_f64;
    typedef octree_f32 octree;
}