#pragma once

//std
#include <memory>
#include <map>

//mandala
#include "types.hpp"

namespace mandala
{
    struct texture_t;

    struct frame_buffer_t
    {
        frame_buffer_t(uint32_t width, uint32_t height);
        ~frame_buffer_t();

        uint32_t id;
		std::shared_ptr<texture_t> color_texture;
		uint32_t depth_stencil_render_buffer;

    private:
        frame_buffer_t(const frame_buffer_t&) = delete;
        frame_buffer_t& operator=(const frame_buffer_t&) = delete;
    };
};


