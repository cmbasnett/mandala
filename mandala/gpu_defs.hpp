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

	typedef rectangle_i32_t	gpu_viewport_type;
	typedef uint8_t			gpu_clear_flag_type;
	typedef vec2_u32_t		gpu_frame_buffer_size_type;
	typedef uint8_t			gpu_frame_buffer_type_flags_type;

	enum : gpu_frame_buffer_type_flags_type
	{
		gpu_frame_buffer_type_flag_color = (1 << 0),
		gpu_frame_buffer_type_flag_depth = (1 << 1),
		gpu_frame_buffer_type_flag_stencil = (1 << 2)
	};

	enum class gpu_frame_buffer_type_e : gpu_frame_buffer_type_flags_type
	{
		color = (gpu_frame_buffer_type_flag_color),
		color_depth = (gpu_frame_buffer_type_flag_color | gpu_frame_buffer_type_flag_depth),
		color_depth_stencil = (gpu_frame_buffer_type_flag_color | gpu_frame_buffer_type_flag_depth | gpu_frame_buffer_type_flag_stencil),
		depth = (gpu_frame_buffer_type_flag_depth),
		depth_stencil = (gpu_frame_buffer_type_flag_depth | gpu_frame_buffer_type_flag_stencil)
	};

    enum class gpu_data_type_e
    {
        byte,
        unsigned_byte,
        short_,
        unsigned_short,
        int_,
        unsigned_int,
        float_,
        double_
    };

    template<typename T>
    struct gpu_data_type_;

    template<>
    struct gpu_data_type_<int8_t>
    {
        static const auto value = gpu_data_type_e::byte;
    };

    template<>
    struct gpu_data_type_<uint8_t>
    {
        static const auto value = gpu_data_type_e::unsigned_byte;
    };

    template<>
    struct gpu_data_type_<int16_t>
    {
        static const auto value = gpu_data_type_e::short_;
    };

    template<>
    struct gpu_data_type_<uint16_t>
    {
        static const auto value = gpu_data_type_e::unsigned_short;
    };

    template<>
    struct gpu_data_type_<int32_t>
    {
        static const auto value = gpu_data_type_e::int_;
    };

    template<>
    struct gpu_data_type_<uint32_t>
    {
        static const auto value = gpu_data_type_e::unsigned_int;
    };

    template<>
    struct gpu_data_type_<float32_t>
    {
        static const auto value = gpu_data_type_e::float_;
    };

    template<>
    struct gpu_data_type_<float64_t>
    {
        static const auto value = gpu_data_type_e::double_;
    };
}