#pragma once

#include <glm\glm.hpp>

namespace glm
{
    // rank
	template <typename valType = std::enable_if<std::is_floating_point<valType>::value>::type>
    GLM_FUNC_QUALIFIER valType percent_rank
        (
        valType const & x,
        valType const & minVal,
        valType const & maxVal
        )
    {
        return clamp((x - minVal) / (maxVal - minVal), valType(0), valType(1));
	}

	template<typename T>
	inline std::istream& operator>>(std::istream& istream, tvec2<T> const& v)
	{
		istream >> v.x;
		istream >> v.y;
	}
}