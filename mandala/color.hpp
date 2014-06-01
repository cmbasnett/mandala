#pragma once

//mandala
#include "types.hpp"

//glm
#include <glm\ext.hpp>

namespace mandala
{
	namespace utils
	{
		vec3_t rgb_to_hsv(const vec3_t& rgb)
		{
			auto rgb_max = glm::compMax(rgb);
			auto rgb_min = glm::compMin(rgb);
			auto delta = rgb_max - rgb_min;

			if (rgb_max == 0)
			{
				return vec3_t(-1, 0, 0);
			}

			float32_t h, s, v = 0;
			
			v = rgb_max;
			s = delta / rgb_max;

			if (rgb.r == rgb_max)
			{
				h = (rgb.g - rgb.b) / delta;
			}
			else if (rgb.g == rgb_max)
			{
				h = 2 + (rgb.b - rgb.r) / delta;
			}
			else
			{
				h = 4 + (rgb.r - rgb.g) / delta;
			}

			h *= 60;

			if (h < 0)
			{
				h += 360;
			}

			return vec3_t(h, s, v);
		}
	};
};
