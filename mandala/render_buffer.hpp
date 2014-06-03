#pragma once

//std
#include <memory>

//mandala
#include "types.hpp"

namespace mandala
{
    struct render_buffer_t
    {
        enum class type_t
        {
            depth,
            stencil
        };

        struct scope_bind_t
        {
            scope_bind_t(const render_buffer_t& render_buffer);
            ~scope_bind_t();
        private:
            const render_buffer_t& render_buffer;
        };

        render_buffer_t(type_t type, uint32_t width, uint32_t height);
        ~render_buffer_t();

        void bind() const;
        void unbind() const;

        int32_t get_width() const;
        int32_t get_height() const;
        int32_t get_internal_format() const;
        int32_t get_red_size() const;
        int32_t get_green_size() const;
        int32_t get_blue_size() const;
        int32_t get_alpha_size() const;
        int32_t get_depth_size() const;
        int32_t get_stencil_size() const;

        uint32_t id = 0;

    private:
        render_buffer_t(const render_buffer_t&) = delete;
        render_buffer_t& operator=(const render_buffer_t&) = delete;
    };
};
