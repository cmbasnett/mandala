#pragma once

//mandala
#include "types.hpp"
#include "rectangle.hpp"

namespace mandala
{
    namespace details
    {
        template<typename Value, Value Default>
        struct value_wrapper_
        {
            typedef Value value_type;
            typedef value_wrapper_<Value, Default> type;

            value_wrapper_() = default;
            value_wrapper_(const value_type& value) :
                value(value)
            {
            }

            operator value_type() const
            {
                return value;
            }

            type& operator=(value_type value)
            {
                this->value = value;

                return *this;
            }

            value_type* operator&()
            {
                return &value;
            }

        private:
            value_type value = Default;
        };
    }

    typedef details::value_wrapper_<u32, 0>     gpu_id;
    typedef details::value_wrapper_<i32, -1>    gpu_location;
    typedef details::value_wrapper_<u32, 0>     gpu_index;

    typedef rectangle_i32   gpu_viewport_type;
    typedef u8              gpu_clear_flag_type;
    typedef vec2_u32        gpu_frame_buffer_size_type;
    typedef u8              gpu_frame_buffer_type_flags_type;

    enum : gpu_frame_buffer_type_flags_type
    {
        GPU_FRAME_BUFFER_TYPE_FLAG_COLOR = (1 << 0),
        GPU_FRAME_BUFFER_TYPE_FLAG_DEPTH = (1 << 1),
        GPU_FRAME_BUFFER_TYPE_FLAG_STENCIL = (1 << 2)
    };

    enum class gpu_frame_buffer_type : gpu_frame_buffer_type_flags_type
    {
        COLOR = (GPU_FRAME_BUFFER_TYPE_FLAG_COLOR),
        COLOR_DEPTH = (GPU_FRAME_BUFFER_TYPE_FLAG_COLOR | GPU_FRAME_BUFFER_TYPE_FLAG_DEPTH),
        COLOR_DEPTH_STENCIL = (GPU_FRAME_BUFFER_TYPE_FLAG_COLOR | GPU_FRAME_BUFFER_TYPE_FLAG_DEPTH | GPU_FRAME_BUFFER_TYPE_FLAG_STENCIL),
        DEPTH = (GPU_FRAME_BUFFER_TYPE_FLAG_DEPTH),
        DEPTH_STENCIL = (GPU_FRAME_BUFFER_TYPE_FLAG_DEPTH | GPU_FRAME_BUFFER_TYPE_FLAG_STENCIL)
    };

    enum class gpu_data_type
    {
        BYTE,
        UNSIGNED_BYTE,
        SHORT,
        UNSIGNED_SHORT,
        INT,
        UNSIGNED_INT,
        FLOAT,
        DOUBLE
    };

    template<typename T>
    struct gpu_data_type_;

    template<>
    struct gpu_data_type_<i8>
    {
        static const auto VALUE = gpu_data_type::BYTE;
    };

    template<>
    struct gpu_data_type_<u8>
    {
        static const auto VALUE = gpu_data_type::UNSIGNED_BYTE;
    };

    template<>
    struct gpu_data_type_<i16>
    {
        static const auto VALUE = gpu_data_type::SHORT;
    };

    template<>
    struct gpu_data_type_<u16>
    {
        static const auto VALUE = gpu_data_type::UNSIGNED_SHORT;
    };

    template<>
    struct gpu_data_type_<i32>
    {
        static const auto VALUE = gpu_data_type::INT;
    };

    template<>
    struct gpu_data_type_<u32>
    {
        static const auto VALUE = gpu_data_type::UNSIGNED_INT;
    };

    template<>
    struct gpu_data_type_<f32>
    {
        static const auto VALUE = gpu_data_type::FLOAT;
    };

    template<>
    struct gpu_data_type_<f64>
    {
        static const auto VALUE = gpu_data_type::DOUBLE;
    };
}