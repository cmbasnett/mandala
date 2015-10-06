#pragma once

//mandala
#include "types.hpp"

namespace mandala
{
    namespace details
    {
        template<typename Scalar, typename Enable = void>
        struct line2;

        template<typename Scalar>
        struct line2<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type>
        {
            typedef Scalar scalar_type;
            typedef line2<scalar_type> type;
            typedef glm::detail::tvec2<scalar_type> value_type;
            typedef line2<f32> real_type;

            value_type start;
            value_type end;

            line2() = default;

            line2(const value_type& start, const value_type& end) :
                start(start),
                end(end)
            {
            }

            real_type::value_type direction() const
            {
                return glm::normalize((real_type::value_type)end - (real_type::value_type)start);
            }

            f32 length() const
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

        template<typename Scalar, typename Real = void, typename Enable = void>
        struct line3;

        template<typename Scalar>
        struct line3<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type>
        {
            typedef Scalar scalar_type;
            typedef line3<scalar_type> type;
            typedef glm::detail::tvec3<scalar_type> value_type;
            typedef line3<f32> real_type;

            value_type start;
            value_type end;

            line3() = default;

            line3(const value_type& start, const value_type& end) :
                start(start),
                end(end)
            {
            }

            real_type::value_type direction() const
            {
                return glm::normalize((real_type::value_type)end - (real_type::value_type)start);
            }

            f32 length() const
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
    }

    typedef details::line2<i8>	line2_i8;
    typedef details::line2<i16> line2_i16;
    typedef details::line2<i32> line2_i32;
    typedef details::line2<i64> line2_i64;
    typedef details::line2<f32> line2_f32;
    typedef details::line2<f64> line2_f64;
    typedef line2_f32			line2;

    typedef details::line3<i8>  line3_i8;
    typedef details::line3<i16> line3_i16;
    typedef details::line3<i32> line3_i32;
    typedef details::line3<i64> line3_i64;
    typedef details::line3<f32> line3_f32;
    typedef details::line3<f64> line3_f64;
    typedef line3_f32			line3;
}
