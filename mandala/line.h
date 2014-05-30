#pragma once

//mandala
#include "types.h"

namespace mandala
{
	namespace details
	{
		template<typename T, typename Enable = void>
		struct line2_t;

		template<typename T>
		struct line2_t<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
		{
			typedef line2_t<T> type;
			typedef glm::detail::tvec2<T> value_type;
            typedef line2_t<float32_t> real_type;

            value_type start;
            value_type end;

			real_type::value_type direction() const
			{
				return glm::normalize((real_type::value_type)end - (real_type::value_type)start);
			}

			float32_t length() const
			{
				return glm::length((real_type::value_type)end - (real_type::value_type)start);
			}

			type operator-(const value_type& t) const
			{
                type sum;
				sum.start = start - t;
				sum.end = end - t;
				return sum;
			}

			type& operator-=(const value_type& t)
			{
				*this = *this - t;
				return *this;
			}

			type operator+(const value_type& t) const
			{
                type sum;
				sum.start = start + t;
				sum.end = end + t;
				return sum;
			}

			type& operator+=(const value_type& t)
			{
				*this = *this + t;
				return *this;
			}
		};

		template<typename T, typename Enable = void>
		struct line3_t;

		template<typename T>
		struct line3_t<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
        {
            typedef line3_t<T> type;
            typedef glm::detail::tvec3<T> value_type;
            typedef line3_t<float32_t> real_type;

			value_type start;
			value_type end;

			real_type::value_type direction() const
			{
				return glm::normalize((real_type::value_type)end - (real_type::value_type)start);
			}

			float32_t length() const
			{
				return glm::length((real_type::value_type)end - (real_type::value_type)start);
			}

            type operator-(const value_type& t) const
			{
                type sum;
				sum.start = start - t;
				sum.end = end - t;
				return sum;
			}

            type& operator-=(const value_type& t)
			{
				*this = *this - t;
				return *this;
			}

            type operator+(const value_type& t) const
			{
				type_t sum;
				sum.start = start + t;
				sum.end = end + t;
				return sum;
			}

            type& operator+=(const value_type& t)
			{
				*this = *this + t;
				return *this;
			}
		};
	};

	typedef details::line2_t<int8_t>	line2_i8_t;
	typedef details::line2_t<int16_t>	line2_i16_t;
	typedef details::line2_t<int32_t>	line2_i32_t;
	typedef details::line2_t<int64_t>	line2_i64_t;
	typedef details::line2_t<float32_t> line2_f32_t;
	typedef details::line2_t<float64_t> line2_f64_t;
	typedef line2_f32_t line2_t;

	typedef details::line3_t<int8_t>	line3_i8_t;
	typedef details::line3_t<int16_t>	line3_i16_t;
	typedef details::line3_t<int32_t>	line3_i32_t;
	typedef details::line3_t<int64_t>	line3_i64_t;
	typedef details::line3_t<float32_t> line3_f32_t;
	typedef details::line3_t<float64_t> line3_f64_t;
	typedef line3_f32_t line3_t;
};