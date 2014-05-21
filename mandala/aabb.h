#pragma once

//std
#include <array>

//mandala
#include "types.h"
#include "plane.h"
#include "macros.h"

namespace mandala
{
	namespace details
	{
		template<typename T, typename Enable = void>
		struct aabb2_t;

		template<typename T>
		struct aabb2_t<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
		{
			typedef aabb2_t<T> type_t;
			typedef glm::detail::tvec2<T> value_type_t;

			static const size_t corner_count = 4;

			value_type_t min;
			value_type_t max;

			aabb2_t()
			{
			}

			aabb2_t(const value_type_t& min, const value_type_t& max) :
				min(min),
				max(max)
			{
			}

			inline T width() const
			{
				return max.x - min.x;
			}

			inline T height() const
			{
				return max.y - min.y;
			}

			inline value_type_t size() const
			{
				return max - min;
			}

			inline value_type_t center() const
			{
				return min + ((max - min) / static_cast<T>(2));
			}

			std::array<value_type_t, corner_count> get_corners() const
			{
				return { min,
						 value_type_t(min.x, max.y),
						 value_type_t(max.x, min.y),
						 max };
			}

			type_t operator-(const value_type_t& t) const
			{
				type_t sum;
				sum.min = min - t;
				sum.max = max - t;
				return sum;
			}

			type_t& operator-=(const value_type_t& t)
			{
				*this = *this - t;
				return *this;
			}

			type_t operator+(const value_type_t& t) const
			{
				type_t sum;
				sum.min = min + t;
				sum.max = max + t;
				return sum;
			}

			type_t& operator+=(const value_type_t& t)
			{
				*this = *this + t;
				return *this;
			}

			static type_t join(const type_t& lhs, const type_t& rhs)
			{
				type_t result;
				result.min = glm::min(lhs.min, rhs.min);
				result.max = glm::max(lhs.max, rhs.max);
				return result;
			}

			type_t& join(const type_t& rhs)
			{
				*this = join(*this, rhs);
				return *this;
			}
		};

		template<typename T, typename Enable = void>
		struct aabb3_t;

		template<typename T>
		struct aabb3_t<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
		{
			typedef aabb3_t<T> type_t;
			typedef glm::detail::tvec3<T> value_type_t;
			typedef plane3_t<float32_t> plane_type_t;

			static const size_t corner_count = 8;
			static const size_t plane_count = 6;

			value_type_t min;
			value_type_t max;

			aabb3_t()
			{
			}

			aabb3_t(const value_type_t& min, const value_type_t& max) :
				min(min),
				max(max)
			{
			}

			value_type_t size() const
			{
				return max - min;
			}

			value_type_t center() const
			{
				return min + ((max - min) / 2);
			}

			std::array<plane_type_t, plane_count> get_planes() const
			{
				return { { min, VEC3_RIGHT },
						 { max, VEC3_LEFT },
						 { min, VEC3_DOWN },
						 { max, VEC3_UP },
						 { min, VEC3_BACKWARD },
						 { max, VEC3_FORWARD } };
			}

			std::array<value_type_t, corner_count> get_corners() const
			{
				return{ min,
						value_type_t(min.x, min.y, max.z),
						value_type_t(min.x, max.y, min.z),
						value_type_t(min.x, max.y, max.z),
						value_type_t(max.x, min.y, min.z),
						value_type_t(max.x, min.y, max.z),
						value_type_t(max.x, max.y, min.z),
						max };
			}

			type_t operator-(const value_type_t& t) const
			{
				type_t sum;
				sum.min = min - t;
				sum.max = max - t;
				return sum;
			}

			type_t& operator-=(const value_type_t& t)
			{
				*this = *this - t;
				return *this;
			}

			type_t operator+(const value_type_t& t) const
			{
				type_t sum;
				sum.min = min + t;
				sum.max = max + t;
				return sum;
			}

			type_t& operator+=(const value_type_t& t)
			{
				*this = *this + t;
				return *this;
			}

			static type_t join(const type_t& lhs, const type_t& rhs)
			{
				type_t result;
				result.min = glm::min(lhs.min, rhs.min);
				result.max = glm::max(lhs.max, rhs.max);
				return result;
			}

			type_t& join(const type_t& rhs)
			{
				*this = join(*this, rhs);
				return *this;
			}
		};
	};

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
};
