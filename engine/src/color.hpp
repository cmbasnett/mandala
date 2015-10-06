#pragma once

//std
#include <iomanip>
#include <sstream>

//glm
#include <glm\ext.hpp>

//mandala
#include "types.hpp"

namespace mandala
{
    inline u32 rgb_to_u32(const vec3& rgb)
    {
        u32 i = 0;

        i |= (static_cast<u32>(rgb.r * 255) << 16);
        i |= (static_cast<u32>(rgb.g * 255) << 8);
        i |= (static_cast<u32>(rgb.b * 255) << 0);

        return i;
    }

    inline u32 rgba_to_u32(const vec4& rgba)
    {
        u32 i = 0;

        i |= (static_cast<u32>(rgba.a * 255) << 24);
        i |= (static_cast<u32>(rgba.r * 255) << 16);
        i |= (static_cast<u32>(rgba.g * 255) << 8);
        i |= (static_cast<u32>(rgba.b * 255) << 0);

        return i;
    }

    inline vec3 u32_to_rgb(u32 i)
    {
        vec3 rgb;

        rgb.r = static_cast<vec3::value_type>((i & 0x00FF0000) >> 16) / 255.0f;
        rgb.g = static_cast<vec3::value_type>((i & 0x0000FF00) >> 8) / 255.0f;
        rgb.b = static_cast<vec3::value_type>((i & 0x000000FF) >> 0) / 255.0f;

        return rgb;
    }

    inline vec4 u32_to_rgba(u32 i)
    {
        vec4 rgb;

        rgb.a = static_cast<vec4::value_type>((i & 0xFF000000) >> 24) / 255.0f;
        rgb.r = static_cast<vec4::value_type>((i & 0x00FF0000) >> 16) / 255.0f;
        rgb.g = static_cast<vec4::value_type>((i & 0x0000FF00) >> 8) / 255.0f;
        rgb.b = static_cast<vec4::value_type>((i & 0x000000FF) >> 0) / 255.0f;

        return rgb;
    }

    static ptrdiff_t rgb_hex_string_length = 6;
    static ptrdiff_t rgba_hex_string_length = 8;

    template<typename Char>
    inline std::basic_string<Char> rgb_to_hex(const vec3& rgb)
    {
        std::basic_ostringstream<Char> ostringstream;
        
        ostringstream << std::hex << std::setw(6) << std::setfill<Char>('0') << rgb_to_u32(rgb);

        return ostringstream.str();
    }

    template<typename Char>
    inline std::basic_string<Char> rgba_to_hex(const vec4& rgba)
    {
        std::basic_ostringstream<Char> ostringstream;

        ostringstream << std::hex << std::setw(8) << std::setfill<Char>('0') << rgba_to_u32(rgb);

        return ostringstream.str();
    }

    template<typename Char>
    inline vec3 hex_to_rgb(const std::basic_string<Char>& hex_string)
    {
        vec3 rgb;

        auto i = std::stoul(hex_string, nullptr, 16);

        rgb.r = static_cast<vec3::value_type>((i & 0xFF0000) >> 16) / 255.0f;
        rgb.g = static_cast<vec3::value_type>((i & 0x00FF00) >> 8) / 255.0f;
        rgb.b = static_cast<vec3::value_type>((i & 0x0000FF) >> 0) / 255.0f;

        return rgb;
    }

    template<typename Char>
    inline vec4 hex_to_rgba(const std::basic_string<Char>& hex_string)
    {
        vec4 rgba;

        auto i = std::stoul(hex_string, nullptr, 16);

        rgba.a = static_cast<vec4::value_type>((i & 0xFF000000) >> 24) / 255.0f;
        rgba.r = static_cast<vec4::value_type>((i & 0x00FF0000) >> 16) / 255.0f;
        rgba.g = static_cast<vec4::value_type>((i & 0x0000FF00) >> 8) / 255.0f;
        rgba.b = static_cast<vec4::value_type>((i & 0x000000FF) >> 0) / 255.0f;

        return rgb;
    }

    inline vec3 rgb_to_hsv(const vec3& rgb)
    {
        auto rgb_max = glm::compMax(rgb);
        auto rgb_min = glm::compMin(rgb);
        auto rgb_diff = rgb_max - rgb_min;

        if (rgb_max == 0)
        {
            return vec3(-1, 0, 0);
        }

        vec3::value_type h, s, v = 0;
            
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

        return vec3(h, s, v);
    }

    const vec3 color_black = vec3(0);
    const vec3 color_white = vec3(1);
    const vec3 color_red = vec3(1, 0, 0);
    const vec3 color_green = vec3(0, 1, 0);
    const vec3 color_blue = vec3(0, 0, 1);
    const vec3 color_yellow = vec3(1, 1, 0);

    //TODO: get the feeling these should be elsewhere
    struct color_push
    {
        color_push() = delete;
        color_push(const color_push&) = delete;
        color_push(const vec3& color) :
            color(color)
        {
        }

        vec3 color;
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
        static const wchar_t VALUE = L'↓';
    };

    template<typename Char>
    struct color_push_character;

    template<>
    struct color_push_character<wchar_t>
    {
        static const wchar_t VALUE = L'↑';
    };
}

template<typename Char>
std::basic_ostringstream<Char>& operator<<(std::basic_ostringstream<Char>& oss, const mandala::color_push& push)
{
    oss << mandala::color_push_character<Char>::value << mandala::rgb_to_hex<Char>(push.color);

    return oss;
}

template<typename Char>
std::basic_ostringstream<Char>& operator<<(std::basic_ostringstream<Char>& oss, const mandala::details::color_pop_t& pop)
{
    oss << mandala::color_pop_character<Char>::value;

    return oss;
}