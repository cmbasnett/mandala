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

    typedef details::value_wrapper_<uint32_t, 0> gpu_id_t;
    typedef details::value_wrapper_<int32_t, -1> gpu_location_t;
    typedef details::value_wrapper_<uint32_t, 0> gpu_index_t;

    typedef rectangle_i32_t gpu_viewport_type;
    typedef uint8_t         gpu_clear_flag_type;
    typedef vec2_u32_t      gpu_frame_buffer_size_type;
    typedef uint8_t         gpu_frame_buffer_type_flags_type;

    enum : gpu_frame_buffer_type_flags_type
    {
        GPU_FRAME_BUFFER_TYPE_FLAG_COLOR = (1 << 0),
        GPU_FRAME_BUFFER_TYPE_FLAG_DEPTH = (1 << 1),
        GPU_FRAME_BUFFER_TYPE_FLAG_STENCIL = (1 << 2)
    };

    enum class gpu_frame_buffer_type_e : gpu_frame_buffer_type_flags_type
    {
        COLOR = (GPU_FRAME_BUFFER_TYPE_FLAG_COLOR),
        COLOR_DEPTH = (GPU_FRAME_BUFFER_TYPE_FLAG_COLOR | GPU_FRAME_BUFFER_TYPE_FLAG_DEPTH),
        COLOR_DEPTH_STENCIL = (GPU_FRAME_BUFFER_TYPE_FLAG_COLOR | GPU_FRAME_BUFFER_TYPE_FLAG_DEPTH | GPU_FRAME_BUFFER_TYPE_FLAG_STENCIL),
        DEPTH = (GPU_FRAME_BUFFER_TYPE_FLAG_DEPTH),
        DEPTH_STENCIL = (GPU_FRAME_BUFFER_TYPE_FLAG_DEPTH | GPU_FRAME_BUFFER_TYPE_FLAG_STENCIL)
    };

    enum class gpu_data_type_e
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
    struct gpu_data_type_<int8_t>
    {
        static const auto VALUE = gpu_data_type_e::BYTE;
    };

    template<>
    struct gpu_data_type_<uint8_t>
    {
        static const auto VALUE = gpu_data_type_e::UNSIGNED_BYTE;
    };

    template<>
    struct gpu_data_type_<int16_t>
    {
        static const auto VALUE = gpu_data_type_e::SHORT;
    };

    template<>
    struct gpu_data_type_<uint16_t>
    {
        static const auto VALUE = gpu_data_type_e::UNSIGNED_SHORT;
    };

    template<>
    struct gpu_data_type_<int32_t>
    {
        static const auto VALUE = gpu_data_type_e::INT;
    };

    template<>
    struct gpu_data_type_<uint32_t>
    {
        static const auto VALUE = gpu_data_type_e::UNSIGNED_INT;
    };

    template<>
    struct gpu_data_type_<float32_t>
    {
        static const auto VALUE = gpu_data_type_e::FLOAT;
    };

    template<>
    struct gpu_data_type_<float64_t>
    {
        static const auto VALUE = gpu_data_type_e::DOUBLE;
    };
}