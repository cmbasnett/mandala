#pragma once

//naga
#include "types.hpp"
#include "aabb.hpp"

namespace naga
{
    namespace details
    {
        template<typename Scalar, typename Enable = void>
        struct Rectangle;

        template<typename Scalar>
		struct Rectangle<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type>
        {
            typedef Scalar ScalarType;
			typedef glm::detail::tvec2<ScalarType> VectorType;
			typedef Rectangle<ScalarType> Type;

			ScalarType x = 0;
			ScalarType y = 0;
			ScalarType width = 0;
			ScalarType height = 0;

			Rectangle() = default;

			template<typename Scalar>
			Rectangle(Scalar x, Scalar y, Scalar width, Scalar height) :
				x(static_cast<ScalarType>(x)),
				y(static_cast<ScalarType>(y)),
				width(static_cast<ScalarType>(width)),
				height(static_cast<ScalarType>(height))
            {
            }
            
			template<typename Scalar>
			Rectangle(const glm::detail::tvec4<Scalar>& v) :
				x(static_cast<ScalarType>(v.x)),
				y(static_cast<ScalarType>(v.y)),
				width(static_cast<ScalarType>(v.z)),
				height(static_cast<ScalarType>(v.w))
            {
            }

			template<typename Scalar>
			Rectangle(const details::AABB2<Scalar>& aabb)
            {
				x = static_cast<ScalarType>(aabb.min.x);
				y = static_cast<ScalarType>(aabb.min.y);
				width = static_cast<ScalarType>(aabb.width());
				height = static_cast<ScalarType>(aabb.height());
            }

			ScalarType area() const
            {
                return width * height;
            }

            VectorType center() const
            {
				return VectorType(x + (width / 2), y + (height / 2));
            }

			VectorType size() const
            {
				return VectorType(width, height);
            }

			VectorType min() const
            {
				return VectorType(x, y);
            }

			VectorType max() const
            {
				return VectorType(x + width, y + height);
            }

            template<typename Scalar>
            operator glm::detail::tvec4<Scalar>() const
            {
                return glm::detail::tvec4<Scalar>(static_cast<Scalar>(x), static_cast<Scalar>(y), static_cast<Scalar>(width), static_cast<Scalar>(height));
            }

			template<typename Scalar>
			operator Rectangle<Scalar>() const
			{
				return Rectangle<Scalar>(static_cast<Scalar>(x), static_cast<Scalar>(y), static_cast<Scalar>(width), static_cast<Scalar>(height));
			}
        };
    }

    typedef details::Rectangle<f32> Rectangle;
}
