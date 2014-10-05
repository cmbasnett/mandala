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
    typedef vec3_t rgb_type;
    typedef vec4_t rgba_type;

    inline uint32_t rgb_to_uint(const rgb_type& rgb)
	{
        //TODO: verify correctness
		uint32_t i = 0;

		i |= (static_cast<uint32_t>(rgb.r * 255) << 16);
		i |= (static_cast<uint32_t>(rgb.g * 255) << 8);
		i |= (static_cast<uint32_t>(rgb.b * 255) << 0);

		return i;
	}

    inline rgb_type uint_to_rgb(uint32_t i)
    {
        //TODO: implement
    }

    template<typename Char>
    inline std::basic_string<Char, std::char_traits<Char>, std::allocator<Char>> rgb_to_hex(const vec3_t& rgb)
	{
        std::basic_ostringstream<Char, std::char_traits<Char>, std::allocator<Char>> ostringstream;
		
		ostringstream << std::hex << std::setw(6) << std::setfill<Char>('0') << rgb_to_uint(rgb);

        auto s = ostringstream.str();

		return ostringstream.str();
	}

    template<typename Char>
    inline vec3_t hex_to_rgb(const std::basic_string<Char, std::char_traits<Char>, std::allocator<Char>>& hex_string)
	{
		vec3_t rgb;

		auto i = std::stoul(hex_string, nullptr, 16);

		rgb.r = static_cast<float32_t>((i & 0xFF0000) >> 16) / 255.0f;
		rgb.g = static_cast<float32_t>((i & 0x00FF00) >> 8) / 255.0f;
		rgb.b = static_cast<float32_t>((i & 0x0000FF) >> 0) / 255.0f;

		return rgb;
	}

    inline rgb_type rgb_to_hsv(const rgb_type& rgb)
	{
		auto rgb_max = glm::compMax(rgb);
		auto rgb_min = glm::compMin(rgb);
		auto delta = rgb_max - rgb_min;

		if (rgb_max == 0)
		{
            return rgb_type(-1, 0, 0);
		}

        rgb_type::value_type h, s, v = 0;
			
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

        return rgb_type(h, s, v);
	}

    const rgb_type color_black = rgb_type(0);
    const rgb_type color_white = rgb_type(1);
    const rgb_type color_red = rgb_type(1, 0, 0);
    const rgb_type color_green = rgb_type(0, 1, 0);
    const rgb_type color_blue = rgb_type(0, 0, 1);
    const rgb_type color_yellow = rgb_type(1, 1, 0);
}
