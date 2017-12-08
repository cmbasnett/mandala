#pragma once

//std
#include <array>

//boost
#include <boost/make_shared.hpp>

//naga
#include "aabb.hpp"

namespace naga
{
    namespace details
    {
        template<typename ScalarType, typename Enable = void>
        struct OctTree;

		template<typename ScalarType>
		struct OctTree<ScalarType, typename std::enable_if<std::is_floating_point<ScalarType>::value>::type>
        {
			typedef details::AABB3<ScalarType> BoundsType;
			typedef OctTree<ScalarType> Type;
			typedef std::array<boost::shared_ptr<Type>, 8> ChildrenType;

			OctTree(ScalarType size = 2048) :
				bounds(BoundsType::VectorType(-size / 2), BoundsType::VectorType(size / 2))
            {
            }

			OctTree(const BoundsType& bounds) :
                bounds(bounds)
            {
            }

            void grow()
            {
				const auto child_bounds = BoundsType(bounds.min, bounds.center());
                const auto child_bounds_size = child_bounds.size();

				children[0] = boost::make_shared<Type>(child_bounds);
				children[1] = boost::make_shared<Type>(child_bounds + BoundsType::VectorType(child_bounds_size.x, 0, 0));
				children[2] = boost::make_shared<Type>(child_bounds + BoundsType::VectorType(0, child_bounds_size.y, 0));
				children[3] = boost::make_shared<Type>(child_bounds + BoundsType::VectorType(child_bounds_size.x, child_bounds_size.y, 0));
				children[4] = boost::make_shared<Type>(child_bounds + BoundsType::VectorType(0, 0, child_bounds_size.z));
				children[5] = boost::make_shared<Type>(child_bounds + BoundsType::VectorType(child_bounds_size.x, 0, child_bounds_size.z));
				children[6] = boost::make_shared<Type>(child_bounds + BoundsType::VectorType(0, child_bounds_size.y, child_bounds_size.z));
				children[7] = boost::make_shared<Type>(child_bounds + child_bounds_size);
            }

            const BoundsType& get_bounds() const { return bounds; }
            const ChildrenType& get_children() const { return children; }
            bool is_leaf() const { return children[0] == nullptr; }

        private:
            BoundsType bounds;
            ChildrenType children;
        };
    }

	typedef details::OctTree<f32> OctTree;
}