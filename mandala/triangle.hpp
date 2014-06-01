#pragma once

//std
#include <array>
#include <type_traits>

//mandala
#include "types.hpp"

namespace mandala
{
	namespace detail
	{
		template<typename T, typename Enable = void>
		struct triangle3_t;

		template<typename T>
		struct triangle3_t<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
		{
			typedef glm::detail::tvec2<T> value_type_t;
			typedef glm::detail::tvec2<float32_t> normal_type_t;
			typedef triangle3_t<value_type_t> type_t;

			static const size_t point_count = 3;

			triangle3_t()
			{
			}

			triangle3_t(const std::array<value_type_t, 3>& points) :
				points(points)
			{
			}

			triangle3_t(const value_type_t& point0, const value_type_t& point1, const value_type_t& point2)
			{
				points[0] = point0;
				points[1] = point1;
				points[2] = point2;
			}

			value_type_t& operator[](size_t index)
			{
				return points[index];
			}

			normal_type_t normal() const
			{
				return glm::cross(v1 - v0, v2 - v0);
			}

		private:
			std::array<value_type_t, point_count> points[3];
		};

		template<typename T, typename Enable = void>
		struct triangle2_t;

		template<typename T>
		struct triangle2_t<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
		{
			typedef glm::detail::tvec2<T> value_type_t;
			typedef triangle2_t<value_type_t> type;

			static const size_t point_count = 3;

			triangle2_t()
			{
			}

			triangle2_t(const std::array<value_type_t, 3>& points) :
				points(points)
			{
			}

			triangle2_t(const value_type_t& point0, const value_type_t& point1, const value_type_t& point2)
			{
				points[0] = point0;
				points[1] = point1;
				points[2] = point2;
			}

			value_type_t& operator[](size_t index)
			{
				return points[index];
			}

			explicit operator triangle3_t<T>() const
			{
				return{ static_cast<triangle3_t<T>>(points[0]), static_cast<triangle3_t<T>>(points[1]), static_cast<triangle3_t<T>>(points[2]) };
			}

		private:
			std::array<value_type_t, point_count> points;
		};
	};

	typedef detail::triangle2_t<int8_t> triangle2_i8_t;
	typedef detail::triangle2_t<int16_t> triangle2_i16_t;
	typedef detail::triangle2_t<int32_t> triangle2_i32_t;
	typedef detail::triangle2_t<int64_t> triangle2_i64_t;
	typedef detail::triangle2_t<float32_t> triangle2_f32_t;
	typedef detail::triangle2_t<float64_t> triangle2_f64_t;
	typedef triangle2_f32_t triangle2_t;

	typedef detail::triangle3_t<int8_t> triangle3_i8_t;
	typedef detail::triangle3_t<int16_t> triangle3_i16_t;
	typedef detail::triangle3_t<int32_t> triangle3_i32_t;
	typedef detail::triangle3_t<int64_t> triangle3_i64_t;
	typedef detail::triangle3_t<float32_t> triangle3_f32_t;
	typedef detail::triangle3_t<float64_t> triangle3_f64_t;
	typedef triangle3_f32_t triangle3_t;
};