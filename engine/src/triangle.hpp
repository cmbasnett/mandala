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
        struct triangle3;

        template<typename T>
        struct triangle3<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
        {
            static const size_t POINT_COUNT = 3;

            typedef glm::detail::tvec2<T> point_type;
            typedef std::array<point_type, POINT_COUNT> points_type;
            typedef glm::detail::tvec2<f32> normal_type;
            typedef triangle3<T> type;

            triangle3() = default;

            triangle3(const points_type& points) :
                points(points)
            {
            }

            triangle3(const point_type& point0, const point_type& point1, const point_type& point2)
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
        struct triangle2;

        template<typename T>
        struct triangle2<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
        {
            static const size_t POINT_COUNT = 3;

            typedef glm::detail::tvec2<T> point_type;
            typedef std::array<point_type, POINT_COUNT> points_type;
            typedef triangle2<T> type;

            triangle2() = default;

            triangle2(const points_type& points) :
                points(points)
            {
            }

            triangle2(const point_type& point0, const point_type& point1, const point_type& point2)
            {
                points[0] = point0;
                points[1] = point1;
                points[2] = point2;
            }

            point_type& operator[](size_t index)
            {
                return points[index];
            }

            explicit operator triangle3<T>() const
            {
                return triangle3<T>(static_cast<triangle3<T>::point_type>(points[0]), static_cast<triangle3<T>::point_type>(points[1]), static_cast<triangle3<T>::point_type>(points[2]));
            }

        private:
            points_type points;
        };
    }

    typedef detail::triangle2<i8> triangle2_i8;
    typedef detail::triangle2<i16> triangle2_i16;
    typedef detail::triangle2<i32> triangle2_i32;
    typedef detail::triangle2<i64> triangle2_i64;
    typedef detail::triangle2<f32> triangle2_f32;
    typedef detail::triangle2<f64> triangle2_f64;
    typedef triangle2_f32 triangle2;

    typedef detail::triangle3<i8> triangle3_i8;
    typedef detail::triangle3<i16> triangle3_i16;
    typedef detail::triangle3<i32> triangle3_i32;
    typedef detail::triangle3<i64> triangle3_i64;
    typedef detail::triangle3<f32> triangle3_f32;
    typedef detail::triangle3<f64> triangle3_f64;
    typedef triangle3_f32 triangle3;
}
