#pragma once

//mandala
#include "types.hpp"

//glm
#include <glm\ext.hpp>

//std
#include <iomanip>
#include <sstream>

namespace mandala
{
	inline uint32_t rgb_to_int(const vec3_t& rgb)
	{
		uint32_t i = 0;

		i |= (static_cast<uint32_t>(rgb.r * 255) << 16);
		i |= (static_cast<uint32_t>(rgb.g * 255) << 8);
		i |= (static_cast<uint32_t>(rgb.b * 255) << 0);

		return i;
	}

	inline std::string rgb_to_hex(const vec3_t& rgb)
	{
		std::ostringstream ostringstream;
		
		ostringstream << std::hex << std::setw(6) << std::setfill('0') << rgb_to_int(rgb);

		auto s = ostringstream.str();

		return ostringstream.str();
	}

	inline vec3_t hex_to_rgb(const std::wstring& hex_string)
	{
		vec3_t rgb;

		auto i = std::stoul(hex_string, nullptr, 16);

		rgb.r = static_cast<float32_t>((i & 0xFF0000) >> 16) / 255.0f;
		rgb.g = static_cast<float32_t>((i & 0x00FF00) >> 8) / 255.0f;
		rgb.b = static_cast<float32_t>((i & 0x0000FF) >> 0) / 255.0f;

		return rgb;
	}

	inline vec3_t rgb_to_hsv(const vec3_t& rgb)
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
}
