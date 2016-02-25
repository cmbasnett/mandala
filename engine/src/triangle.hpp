#pragma once

//std
#include <array>
#include <type_traits>

//naga
#include "types.hpp"

namespace naga
{
    namespace details
    {
        template<typename T, typename Enable = void>
        struct triangle3;

        template<typename T>
        struct triangle3<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
        {
            static const size_t POINT_COUNT = 3;

            typedef glm::detail::tvec3<T> point_type;
            typedef std::array<point_type, POINT_COUNT> points_type;
            typedef glm::detail::tvec3<f32> normal_type;
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
				return glm::cross(static_cast<normal_type>(v1 - v0), static_cast<normal_type>(v2 - v0));
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

	//template<typename P, typename T>
	//vec2 barycentric(const glm::detail::tvec2<P>& p, const details::triangle2<T>& t)
	//{
	//	auto v0 = static_cast<vec2>(t[1] - t[0]);
	//	auto v1 = static_cast<vec2>(t[2] - t[0]);
	//	auto v2 = static_cast<vec2>(p)-static_cast<vec2>(t[0]);
	//	auto d00 = glm::dot(v0, v0);
	//	auto d01 = glm::dot(v0, v1);
	//	auto d11 = glm::dot(v1, v1);
	//	auto d20 = glm::dot(v2, v0);
	//	auto d21 = glm::dot(v2, v1);
	//	auto denom = d00 * d11 - d01 * d01;
	//	vec3 b;
	//	b.y = (d11 * d20 - d01 * d21) / denom;
	//	b.z = (d00 * d21 - d01 * d20) / denom;
	//	b.x = 1.0f - r.y - r.z;
	//	return b;
	//}

	//template<typename P, typename T>
	//vec3 barycentric(const details::triangle3<P>::point_type& p, const details::triangle3<T>& t)
	//{
	//	auto v0 = static_cast<vec3>(t[1] - t[0]);
	//	auto v1 = static_cast<vec3>(t[2] - t[0]);
	//	auto v2 = static_cast<vec3>(p) - static_cast<vec3>(t[0]);
	//	auto d00 = glm::dot(v0, v0);
	//	auto d01 = glm::dot(v0, v1);
	//	auto d11 = glm::dot(v1, v1);
	//	auto d20 = glm::dot(v2, v0);
	//	auto d21 = glm::dot(v2, v1);
	//	auto denom = d00 * d11 - d01 * d01;
	//	vec3 b;
	//	b.y = (d11 * d20 - d01 * d21) / denom;
	//	b.z = (d00 * d21 - d01 * d20) / denom;
	//	b.x = 1.0f - r.y - r.z;
	//	return b;
	//}

    typedef details::triangle2<i8> triangle2_i8;
    typedef details::triangle2<i16> triangle2_i16;
    typedef details::triangle2<i32> triangle2_i32;
    typedef details::triangle2<i64> triangle2_i64;
    typedef details::triangle2<f32> triangle2_f32;
    typedef details::triangle2<f64> triangle2_f64;
    typedef triangle2_f32 triangle2;

    typedef details::triangle3<i8> triangle3_i8;
    typedef details::triangle3<i16> triangle3_i16;
    typedef details::triangle3<i32> triangle3_i32;
    typedef details::triangle3<i64> triangle3_i64;
    typedef details::triangle3<f32> triangle3_f32;
    typedef details::triangle3<f64> triangle3_f64;
    typedef triangle3_f32 triangle3;
}
