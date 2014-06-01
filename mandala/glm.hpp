#pragma once

#include <glm\glm.hpp>

namespace glm
{
	// rank
	template <typename valType>
	GLM_FUNC_QUALIFIER valType percent_rank
		(
		valType const & x,
		valType const & minVal,
		valType const & maxVal
		)
	{
		GLM_STATIC_ASSERT(
			detail::type<valType>::is_float, "'percent_rank' only accept floating-point numbers");

		return clamp((x - minVal) / (maxVal - minVal), valType(0), valType(1));
	}
};