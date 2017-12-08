#pragma once

//std
#include <array>

//naga
#include "types.hpp"
#include "plane.hpp"
#include "range.hpp"

namespace naga
{
    namespace details
    {
        template<typename Scalar, typename Enable = void>
		struct AABB2;

        template<typename Scalar>
		struct AABB2<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type> : Range<glm::detail::tvec2<Scalar>>
        {
            typedef Scalar ScalarType;
			typedef AABB2<ScalarType> Type;
			typedef glm::detail::tvec2<ScalarType> VectorType;

            AABB2() = default;
			AABB2(const VectorType& min, const VectorType& max) :
                Range(min, max)
            {
            }

			inline ScalarType width() const
            {
                return max.x - min.x;
            }

			inline ScalarType height() const
            {
                return max.y - min.y;
            }

			inline VectorType size() const
            {
                return max - min;
            }

			inline VectorType center() const
            {
				return min + (size() / static_cast<ScalarType>(2));
            }

			inline VectorType extents() const
            {
                return (max - min) / ScalarType(2);
            }

			std::array<VectorType, 4> get_corners() const
            {
                return { min,
						 VectorType(min.x, max.y),
						 VectorType(max.x, min.y),
                         max };
            }

			Type operator-(const VectorType& rhs) const
            {
				return Type(min - rhs, max - rhs);
            }

			Type& operator-=(const VectorType& rhs)
            {
                *this = *this - rhs;

                return *this;
            }

			Type operator+(const VectorType& rhs) const
            {
                return Type(min + rhs, max + rhs);
            }

			Type& operator+=(const VectorType& rhs)
            {
                *this = *this + rhs;

                return *this;
            }

			static Type join(const Type& lhs, const Type& rhs)
            {
				return Type(glm::min(lhs.min, rhs.min), glm::max(lhs.max, rhs.max));
            }

			Type& join(const Type& rhs)
            {
                *this = join(*this, rhs);

                return *this;
            }

            template<typename Scalar>
			bool operator==(const AABB2<Scalar>& rhs) const
            {
                return min == rhs.min && max == rhs.max;
            }

            template<typename Scalar>
			bool operator!=(const AABB2<Scalar>& rhs) const
            {
                return min != rhs.min || max != rhs.max;
            }

            template<typename Scalar>
			Type& operator=(const AABB2<Scalar>& rhs) const
            {
                min = static_cast<vector_type>(rhs.min);
                max = static_cast<vector_type>(rhs.max);

                return *this;
            }

            template<typename T>
            operator AABB2<T>()
            {
				return AABB2<T>(static_cast<AABB2<T>::VectorType>(min), static_cast<AABB2<T>::VectorType>(max));
            }

			Type operator<<(const Type& rhs) const
            {
                return join(*this, rhs);
            }

			Type& operator<<=(const Type& rhs)
            {
                return this->join(rhs);
            }
        };

        template<typename T, typename Enable = void>
		struct AABB3;

        template<typename Scalar>
        struct AABB3<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type> : Range<glm::detail::tvec3<Scalar>>
        {
            typedef Scalar ScalarType;
			typedef AABB3<ScalarType> Type;
            typedef Plane3<f32> PlaneType;
			typedef glm::detail::tvec3<ScalarType> VectorType;

            static const size_t CORNER_COUNT = 8;
            static const size_t PLANE_COUNT = 6;

			AABB3() = default;
			AABB3(const VectorType& min, const VectorType& max) :
                Range(min, max)
            {
            }

			inline ScalarType width() const
            {
                return max.x - min.x;
            }

			inline ScalarType height() const
            {
                return max.y - min.y;
            }

			inline ScalarType depth() const
            {
                return max.z - min.z;
            }

            VectorType size() const
            {
                return max - min;
            }

            VectorType center() const
            {
                return min + ((max - min) / ScalarType(2));
            }

			VectorType extents() const
            {
				return (max - min) / ScalarType(2);
            }

            std::array<PlaneType, 6> get_planes() const
            {
                return { { min, VEC3_RIGHT },
                         { max, VEC3_LEFT },
                         { min, VEC3_DOWN },
                         { max, VEC3_UP },
                         { min, VEC3_BACKWARD },
                         { max, VEC3_FORWARD } };
            }

            std::array<VectorType, 8> get_corners() const
            {
                return{ min,
                        VectorType(min.x, min.y, max.z),
						VectorType(min.x, max.y, min.z),
						VectorType(min.x, max.y, max.z),
						VectorType(max.x, min.y, min.z),
						VectorType(max.x, min.y, max.z),
						VectorType(max.x, max.y, min.z),
                        max };
            }

			Type operator-(const VectorType& rhs) const
            {
                return Type(min - rhs, max - rhs);
            }

			Type& operator-=(const VectorType& rhs)
            {
                *this = *this - rhs;

                return *this;
            }

			Type operator+(const VectorType& rhs) const
            {
                return Type(min + rhs, max + rhs);
            }

			Type& operator+=(const VectorType& rhs)
            {
                *this = *this + rhs;

                return *this;
            }

			static Type join(const Type& lhs, const VectorType& rhs)
            {
                return Type(glm::min(lhs.min, rhs), glm::max(lhs.max, rhs));
            }

			static Type join(const Type& lhs, const Type& rhs)
            {
                return Type(glm::min(lhs.min, rhs.min), glm::max(lhs.max, rhs.max));
            };

            Type& join(const VectorType& rhs)
            {
                *this = join(*this, rhs);

                return *this;
            }

			Type& join(const Type& rhs)
            {
                *this = join(*this, rhs);

                return *this;
            }

            template<typename U>
			bool operator==(const AABB3<U>& rhs) const
            {
                return min == rhs.min && max == rhs.max;
            }

            template<typename U>
			bool operator!=(const AABB3<U>& rhs) const
            {
                return min != rhs.min || max != rhs.max;
            }

            template<typename U>
            Type& operator=(const AABB3<U>& rhs)
            {
				min = static_cast<VectorType>(rhs.min);
				max = static_cast<VectorType>(rhs.max);
                return *this;
            }

            template<typename PointScalar, size_t N>
			Type& operator=(const std::array<glm::detail::tvec3<PointScalar>, N>& points)
            {
				min = VectorType(std::numeric_limits<ScalarType>::max());
				max = VectorType(-std::numeric_limits<ScalarType>::max());

                for (const auto& point : points)
                {
                    *this <<= point;
                }

                return *this;
            }

			Type operator<<(const Type& rhs) const
            {
                return join(*this, rhs);
            }

			Type operator<<(const VectorType& rhs) const
            {
                return join(*this, rhs);
            }

			Type& operator<<=(const VectorType& rhs)
            {
                return this->join(rhs);
            }

			Type& operator<<=(const Type& rhs)
            {
                return this->join(rhs);
            }

			Type operator<<(const glm::detail::tmat4x4<ScalarType>& rhs) const
            {
				Type aabb;
				aabb.min = VectorType(std::numeric_limits<ScalarType>::max());
				aabb.max = VectorType(-std::numeric_limits<ScalarType>::max());

                for (const auto& corner : get_corners())
                {
                    auto corner_transformed = glm::detail::tvec4<ScalarType>(corner, 1);
                    corner_transformed = rhs * corner_transformed;

					aabb <<= VectorType(corner_transformed.x, corner_transformed.y, corner_transformed.z);
                }

                return aabb;
            }

            Type& operator<<=(const glm::detail::tmat4x4<ScalarType>& rhs)
            {
                *this = *this << rhs;

                return *this;
            }
        };
    }

	typedef details::AABB2<f32> AABB2;
    typedef details::AABB3<f32> AABB3;
}
