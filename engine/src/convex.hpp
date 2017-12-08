#pragma once

// std
#include <vector>
#include <algorithm>
#include <iterator>

// naga
#include "types.hpp"

#include <boost\python.hpp>

namespace naga
{
	template<typename T>
	int ccw(const glm::detail::tvec2<T>& a, const glm::detail::tvec2<T>& b, const glm::detail::tvec2<T>& c)
	{
		auto area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
		return area > 0 ? -1 : area < 0 ? 1 : 0;
	}

	template<typename T>
	std::vector<glm::detail::tvec2<T>> get_convex_hull(std::vector<glm::detail::tvec2<T>> points)
	{
		typedef glm::detail::tvec2<T> VectorType;

		if (points.size() < 3)
		{
			throw std::invalid_argument("not enough vertices");
		}

		const auto& lowest = std::min_element(points.begin(), points.end(), [&](const VectorType& lhs, const VectorType& rhs)
		{
			return lhs.y == rhs.y ? lhs.x < rhs.x : lhs.y < rhs.y;
		});

		std::iter_swap(points.begin(), lowest);
		std::sort(points.begin() + 1, points.end(), [&](const VectorType& lhs, const VectorType& rhs)
		{
			auto order = ccw(points[0], lhs, rhs);
			return order == 0 ? glm::length(points[0] - lhs) < glm::length(points[0] - rhs) : order == -1;
		});

		std::deque<VectorType> hull;
		hull.push_back(points[0]);
		hull.push_back(points[1]);
		hull.push_back(points[2]);

		for (size_t i = 3; i < points.size(); ++i)
		{
			auto top = hull.back();
			hull.pop_back();

			while (ccw(hull.back(), top, points[i]) != -1)
			{
				top = hull.back();
				hull.pop_back();
			}

			hull.push_back(top);
			hull.push_back(points[i]);
		}

		return std::vector<VectorType>(hull.begin(), hull.end());
	}
}
