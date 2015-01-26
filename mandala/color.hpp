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
    inline uint32_t rgb_to_uint(const rgb_type& rgb)
	{
		uint32_t i = 0;

		i |= (static_cast<uint32_t>(rgb.r * 255) << 16);
		i |= (static_cast<uint32_t>(rgb.g * 255) << 8);
		i |= (static_cast<uint32_t>(rgb.b * 255) << 0);

		return i;
    }

    inline uint32_t rgba_to_uint(const rgba_type& rgba)
    {
        uint32_t i = 0;

        i |= (static_cast<uint32_t>(rgba.a * 255) << 24);
        i |= (static_cast<uint32_t>(rgba.r * 255) << 16);
        i |= (static_cast<uint32_t>(rgba.g * 255) << 8);
        i |= (static_cast<uint32_t>(rgba.b * 255) << 0);

        return i;
    }

    inline rgb_type uint_to_rgb(uint32_t i)
    {
        rgb_type rgb;

        rgb.r = static_cast<rgb_type::value_type>((i & 0x00FF0000) >> 16) / 255.0f;
        rgb.g = static_cast<rgb_type::value_type>((i & 0x0000FF00) >> 8) / 255.0f;
        rgb.b = static_cast<rgb_type::value_type>((i & 0x000000FF) >> 0) / 255.0f;

        return rgb;
    }

    inline rgba_type uint_to_rgba(uint32_t i)
    {
        rgba_type rgb;

        rgb.a = static_cast<rgba_type::value_type>((i & 0xFF000000) >> 24) / 255.0f;
        rgb.r = static_cast<rgba_type::value_type>((i & 0x00FF0000) >> 16) / 255.0f;
        rgb.g = static_cast<rgba_type::value_type>((i & 0x0000FF00) >> 8) / 255.0f;
        rgb.b = static_cast<rgba_type::value_type>((i & 0x000000FF) >> 0) / 255.0f;

        return rgb;
    }

	static ptrdiff_t rgb_hex_string_length = 6;
	static ptrdiff_t rgba_hex_string_length = 8;

    template<typename Char>
    inline std::basic_string<Char, std::char_traits<Char>, std::allocator<Char>> rgb_to_hex(const rgb_type& rgb)
	{
        std::basic_ostringstream<Char, std::char_traits<Char>, std::allocator<Char>> ostringstream;
		
		ostringstream << std::hex << std::setw(6) << std::setfill<Char>('0') << rgb_to_uint(rgb);

		return ostringstream.str();
    }

    template<typename Char>
    inline std::basic_string<Char, std::char_traits<Char>, std::allocator<Char>> rgba_to_hex(const rgba_type& rgba)
    {
        std::basic_ostringstream<Char, std::char_traits<Char>, std::allocator<Char>> ostringstream;

        ostringstream << std::hex << std::setw(8) << std::setfill<Char>('0') << rgba_to_uint(rgb);

        return ostringstream.str();
    }

    template<typename Char>
    inline rgb_type hex_to_rgb(const std::basic_string<Char, std::char_traits<Char>, std::allocator<Char>>& hex_string)
	{
        rgb_type rgb;

		auto i = std::stoul(hex_string, nullptr, 16);

        rgb.r = static_cast<rgb_type::value_type>((i & 0xFF0000) >> 16) / 255.0f;
        rgb.g = static_cast<rgb_type::value_type>((i & 0x00FF00) >> 8) / 255.0f;
        rgb.b = static_cast<rgb_type::value_type>((i & 0x0000FF) >> 0) / 255.0f;

		return rgb;
    }

    template<typename Char>
    inline rgba_type hex_to_rgba(const std::basic_string<Char, std::char_traits<Char>, std::allocator<Char>>& hex_string)
    {
        rgba_type rgba;

        auto i = std::stoul(hex_string, nullptr, 16);

        rgba.a = static_cast<rgba_type::value_type>((i & 0xFF000000) >> 24) / 255.0f;
        rgba.r = static_cast<rgba_type::value_type>((i & 0x00FF0000) >> 16) / 255.0f;
        rgba.g = static_cast<rgba_type::value_type>((i & 0x0000FF00) >> 8) / 255.0f;
        rgba.b = static_cast<rgba_type::value_type>((i & 0x000000FF) >> 0) / 255.0f;

        return rgb;
    }

    inline rgb_type rgb_to_hsv(const rgb_type& rgb)
	{
		auto rgb_max = glm::compMax(rgb);
		auto rgb_min = glm::compMin(rgb);
		auto rgb_diff = rgb_max - rgb_min;

		if (rgb_max == 0)
		{
            return rgb_type(-1, 0, 0);
		}

        rgb_type::value_type h, s, v = 0;
			
		v = rgb_max;
		s = rgb_diff / rgb_max;

		if (rgb.r == rgb_max)
		{
			h = (rgb.g - rgb.b) / rgb_diff;
		}
		else if (rgb.g == rgb_max)
		{
			h = 2 + (rgb.b - rgb.r) / rgb_diff;
		}
		else
		{
			h = 4 + (rgb.r - rgb.g) / rgb_diff;
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

	struct color_push
	{
		color_push() = delete;
		color_push(const color_push&) = delete;
		color_push(const rgb_type& color) :
			color(color)
		{
		}

		rgb_type color;
	};

	namespace details
	{
		struct color_pop_t
		{
		};
	}

	const details::color_pop_t color_pop;

	template<typename Char>
	struct color_pop_character;

	template<>
	struct color_pop_character<wchar_t>
	{
		static const wchar_t value = L'↓';
	};

	template<typename Char>
	struct color_push_character;

	template<>
	struct color_push_character<wchar_t>
	{
		static const wchar_t value = L'↑';
	};
}

template<typename Char>
std::basic_ostringstream<Char, std::char_traits<Char>, std::allocator<Char>>& operator<<(std::basic_ostringstream<Char, std::char_traits<Char>, std::allocator<Char>>& oss, const mandala::color_push& push)
{
	oss << mandala::color_push_character<Char>::value << mandala::rgb_to_hex<Char>(push.color);

	return oss;
}

template<typename Char>
std::basic_ostringstream<Char, std::char_traits<Char>, std::allocator<Char>>& operator<<(std::basic_ostringstream<Char, std::char_traits<Char>, std::allocator<Char>>& oss, const mandala::details::color_pop_t& pop)
{
	oss << mandala::color_pop_character<Char>::value;

	return oss;
}