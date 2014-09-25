#pragma once

//std
#include <array>

//mandala
#include "types.hpp"
#include "plane.hpp"

#include <boost\range\any_range.hpp>

namespace mandala
{
	namespace details
	{
		template<typename Scalar, typename Enable = void>
		struct aabb2_t;

		template<typename Scalar>
		struct aabb2_t<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type>
		{
			typedef Scalar scalar_type;
			typedef aabb2_t<scalar_type> type;
			typedef glm::detail::tvec2<scalar_type> vector_type;

			static const size_t corner_count = 4;

			vector_type min;
			vector_type max;

			aabb2_t()
			{
			}

			aabb2_t(const vector_type& min, const vector_type& max) :
				min(min),
				max(max)
			{
			}

			inline scalar_type width() const
			{
				return max.x - min.x;
			}

			inline scalar_type height() const
			{
				return max.y - min.y;
			}

			inline vector_type size() const
			{
				return max - min;
			}

			inline vector_type center() const
			{
				return min + (size() / static_cast<scalar_type>(2));
			}

			std::array<vector_type, corner_count> get_corners() const
			{
				return { min,
						 vector_type(min.x, max.y),
						 vector_type(max.x, min.y),
						 max };
			}

			type operator-(const vector_type& rhs) const
			{
				return type(min - t, max - t);
			}

			type& operator-=(const vector_type& rhs)
			{
				*this = *this - rhs;
				return *this;
			}

			type operator+(const vector_type& rhs) const
			{
				return type(min + rhs, max + rhs);
			}

			type& operator+=(const vector_type& rhs)
			{
				*this = *this + rhs;
				return *this;
			}

            static type join(const type& lhs, const type& rhs)
			{
				return type(glm::min(lhs.min, rhs.min), glm::max(lhs.max, rhs.max));
			}

            type& join(const type& rhs)
			{
				*this = join(*this, rhs);
				return *this;
			}

			template<typename Scalar>
			bool operator==(const aabb2_t<Scalar>& rhs) const
			{
				return min == rhs.min && max == rhs.max;
			}

			template<typename Scalar>
			bool operator!=(const aabb2_t<Scalar>& rhs) const
			{
				return min != rhs.min || max != rhs.max;
			}

			template<typename Scalar>
			type& operator=(const aabb2_t<Scalar>& rhs) const
			{
				min = static_cast<vector_type>(rhs.min);
				max = static_cast<vector_type>(rhs.max);
				return *this;
			}

			template<typename T>
			operator aabb2_t<T>()
			{
				return aabb2_t<T>(static_cast<aabb2_t<T>::vector_type>(min), static_cast<aabb2_t<T>::vector_type>(max));
			}
		};

		template<typename T, typename Enable = void>
		struct aabb3_t;

		template<typename Scalar>
		struct aabb3_t<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type>
		{
			typedef Scalar scalar_type;
			typedef aabb3_t<scalar_type> type;
			typedef glm::detail::tvec3<scalar_type> vector_type;
			typedef plane3_t<float32_t> plane_type;

			static const size_t corner_count = 8;
			static const size_t plane_count = 6;

			vector_type min;
			vector_type max;

			aabb3_t()
			{
			}

			aabb3_t(const vector_type& min, const vector_type& max) :
				min(min),
				max(max)
			{
			}

			vector_type size() const
			{
				return max - min;
			}

			vector_type center() const
			{
				return min + ((max - min) / scalar_type(2));
			}

			std::array<plane_type, plane_count> get_planes() const
			{
				return { { min, VEC3_RIGHT },
						 { max, VEC3_LEFT },
						 { min, VEC3_DOWN },
						 { max, VEC3_UP },
						 { min, VEC3_BACKWARD },
						 { max, VEC3_FORWARD } };
			}

			std::array<vector_type, corner_count> get_corners() const
			{
				return{ min,
						vector_type(min.x, min.y, max.z),
						vector_type(min.x, max.y, min.z),
						vector_type(min.x, max.y, max.z),
						vector_type(max.x, min.y, min.z),
						vector_type(max.x, min.y, max.z),
						vector_type(max.x, max.y, min.z),
						max };
			}

			type operator-(const vector_type& rhs) const
			{
				return type(min - t, max - t);
			}

			type& operator-=(const vector_type& rhs)
			{
				*this = *this - t;
				return *this;
			}

			type operator+(const vector_type& rhs) const
			{
				return type(min + t, max + t);
			}

			type& operator+=(const vector_type& rhs)
			{
				*this = *this + t;
				return *this;
			}

			static type join(const type& lhs, const type& rhs)
			{
				return type(glm::min(lhs.min, rhs.min), glm::max(lhs.max, rhs.max));
			};

			type& join(const type& rhs)
			{
				*this = join(*this, rhs);
				return *this;
			}

			template<typename U>
			bool operator==(const aabb3_t<U>& rhs) const
			{
				return min == rhs.min && max == rhs.max;
			}

			template<typename U>
			bool operator!=(const aabb3_t<U>& rhs) const
			{
				return min != rhs.min || max != rhs.max;
			}

			template<typename U>
			type& operator=(const aabb3_t<U>& rhs)
			{
				min = static_cast<vector_type>(rhs.min);
				max = static_cast<vector_type>(rhs.max);
				return *this;
			}

			template<typename PointScalar, size_t N>
			type& operator=(const std::array<glm::detail::tvec3<PointScalar>, N>& points)
			{
				min = vector_type(std::numeric_limits<scalar_type>::max());
				max = vector_type(-std::numeric_limits<scalar_type>::max());

				for (const auto& point : points)
				{
					min = glm::min(min, point);
					max = glm::max(max, point);
				}

				return *this;
			}
		};
	}

	typedef details::aabb2_t<int8_t>	aabb2_i8_t;
	typedef details::aabb2_t<int16_t>	aabb2_i16_t;
	typedef details::aabb2_t<int32_t>	aabb2_i32_t;
	typedef details::aabb2_t<int64_t>	aabb2_i64_t;
	typedef details::aabb2_t<float32_t>	aabb2_f32_t;
	typedef details::aabb2_t<float64_t>	aabb2_f36_t;
	typedef aabb2_f32_t					aabb2_t;

	typedef details::aabb3_t<int8_t>	aabb3_i8_t;
	typedef details::aabb3_t<int16_t>	aabb3_i16_t;
	typedef details::aabb3_t<int32_t>	aabb3_i32_t;
	typedef details::aabb3_t<int64_t>	aabb3_i64_t;
	typedef details::aabb3_t<float32_t>	aabb3_f32_t;
	typedef details::aabb3_t<float64_t>	aabb3_f36_t;
	typedef aabb3_f32_t					aabb3_t;
}
