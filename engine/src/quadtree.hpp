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
        struct quadtree_;

        template<typename Scalar>
        struct quadtree_<Scalar, typename std::enable_if<std::is_floating_point<Scalar>::value>::type>
        {
            typedef Scalar scalar_type;
            typedef aabb2_t<scalar_type> bounds_type;
            typedef quadtree_<scalar_type> type;
            typedef std::array<boost::shared_ptr<type>, 4> children_type;

            quadtree_(scalar_type size) :
                bounds(bounds_type::value_type(-size / 2), bounds_type::value_type(size / 2))
            {
            }

            quadtree_(const bounds_type& bounds) :
                bounds(bounds)
            {
            }

            void birth()
            {
                const auto child_bounds = bounds_type(bounds.min, bounds.center());
                const auto child_bounds_size = child_bounds.size();

                children[0] = boost::make_shared<type>(child_bounds);
                children[1] = boost::make_shared<type>(child_bounds + bounds_type::value_type(child_bounds_size.x, 0));
                children[1] = boost::make_shared<type>(child_bounds + bounds_type::value_type(0, child_bounds_size.y));
                children[3] = boost::make_shared<type>(child_bounds + child_bounds_size);
            }

            const bounds_type& get_bounds() const { return bounds; }
            const children_type& get_children() const { return children; }

        private:
            bounds_type bounds;
            children_type children;
        };
    }

    typedef details::quadtree_<float32_t> quadtree_f32_t;
    typedef details::quadtree_<float64_t> quadtree_f64_t;
}