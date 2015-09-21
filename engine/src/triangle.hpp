#pragma once

//std
#include <array>
#include <type_traits>

//mandala
#include "types.hpp"

namespace mandala
{
    namespace details
    {
        template<typename T, typename Enable = void>
        struct triangle3_t;

        template<typename T>
        struct triangle3_t<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
        {
            static const size_t POINT_COUNT = 3;

            typedef glm::detail::tvec2<T> point_type;
            typedef std::array<point_type, POINT_COUNT> points_type;
            typedef glm::detail::tvec2<float32_t> normal_type;
            typedef triangle3_t<T> type;

            triangle3_t() = default;

            triangle3_t(const points_type& points) :
                points(points)
            {
            }

            triangle3_t(const point_type& point0, const point_type& point1, const point_type& point2)
            {
                points[0] = point0;
                points[1] = point1;
                points[2] = point2;
            }

            point_type& operator[](size_t index) const
            {
                return points[index];
            }

            normal_type normal() const
            {
                return glm::cross(v1 - v0, v2 - v0);
            }

        private:
            points_type points;
        };

        template<typename T, typename Enable = void>
        struct triangle2_t;

        template<typename T>
        struct triangle2_t<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
        {
            static const size_t POINT_COUNT = 3;

            typedef glm::detail::tvec2<T> point_type;
            typedef std::array<point_type, POINT_COUNT> points_type;
            typedef triangle2_t<T> type;

            triangle2_t() = default;

            triangle2_t(const points_type& points) :
                points(points)
            {
            }

            triangle2_t(const point_type& point0, const point_type& point1, const point_type& point2)
            {
                points[0] = point0;
                points[1] = point1;
                points[2] = point2;
            }

            point_type& operator[](size_t index)
            {
                return points[index];
            }

            explicit operator triangle3_t<T>() const
            {
                return triangle3_t<T>(static_cast<triangle3_t<T>::point_type>(points[0]), static_cast<triangle3_t<T>::point_type>(points[1]), static_cast<triangle3_t<T>::point_type>(points[2]));
            }

        private:
            points_type points;
        };
    }

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
}
