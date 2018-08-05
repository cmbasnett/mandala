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
        template<typename Scalar, typename Enable = void>
        struct Triangle3;

		template<typename Scalar>
		struct Triangle3<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type>
        {
			typedef Scalar ScalarType;
			typedef glm::tvec3<ScalarType> PointType;
			typedef std::array<PointType, 3> PointsType;
            typedef vec3 NormalType;
			typedef Triangle3<ScalarType> Type;

			Triangle3() = default;

			Triangle3(const PointsType& points) :
                points(points)
            {
            }

			Triangle3(const PointType& point0, const PointType& point1, const PointType& point2)
            {
                points[0] = point0;
                points[1] = point1;
                points[2] = point2;
            }

            PointType& operator[](size_t index)
            {
                return points[index];
            }

			NormalType normal() const
			{
				return glm::cross(static_cast<NormalType>(points[1] - points[0]), static_cast<NormalType>(points[2] - points[0]));
			}

        private:
            PointsType points;
        };

		template<typename Scalar, typename Enable = void>
        struct Triangle2;

		template<typename Scalar>
		struct Triangle2<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type>
        {
			typedef Scalar ScalarType;
			typedef glm::tvec2<ScalarType> PointType;
			typedef std::array<PointType, 3> PointsType;
			typedef Triangle2<ScalarType> Type;

			Triangle2() = default;

			Triangle2(const PointsType& points) :
                points(points)
            {
            }

			Triangle2(const PointType& point0, const PointType& point1, const PointType& point2)
            {
                points[0] = point0;
                points[1] = point1;
                points[2] = point2;
            }

			PointType& operator[](size_t index)
            {
                return points[index];
            }

            explicit operator Triangle3<ScalarType>() const
            {
				return Triangle3<ScalarType>(static_cast<Triangle3<ScalarType>::point_type>(points[0]),
											 static_cast<Triangle3<ScalarType>::point_type>(points[1]),
											 static_cast<Triangle3<ScalarType>::point_type>(points[2]));
            }

        private:
            PointsType points;
        };
	}

	//template<typename P, typename T>
	//vec2 barycentric(const glm::tvec2<P>& p, const details::triangle2<T>& t)
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

	typedef details::Triangle2<f32> Triangle2;
	typedef details::Triangle3<f32> Triangle3;
}
