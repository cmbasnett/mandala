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

			value_type value = Default;

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
		};
	}

	typedef details::value_wrapper_<uint32_t, 0> gpu_id_t;
	typedef details::value_wrapper_<int32_t, -1> gpu_location_t;
	typedef details::value_wrapper_<uint32_t, 0> gpu_index_t;

	typedef rectangle_i32_t	gpu_viewport_type;
	typedef uint8_t			gpu_clear_flag_type;
	typedef vec2_u32_t		gpu_frame_buffer_size_type;
	typedef uint8_t			gpu_frame_buffer_type_flags_type;

	template<typename T>
	struct gpu_data_type_e
	{
	};

	template<>
	struct gpu_data_type_e<int8_t>
	{
		static const uint32_t value = 0x1400;	//GL_BYTE
	};

	template<>
	struct gpu_data_type_e<uint8_t>
	{
		static const uint32_t value = 0x1401;	//GL_UNSIGNED_BYTE
	};

	template<>
	struct gpu_data_type_e<int16_t>
	{
		static const uint32_t value = 0x1402;	//GL_SHORT
	};

	template<>
	struct gpu_data_type_e<uint16_t>
	{
		static const uint32_t value = 0x1403;	//GL_UNSIGNED_SHORT
	};

	template<>
	struct gpu_data_type_e<int32_t>
	{
		static const uint32_t value = 0x1404;	//GL_INT
	};

	template<>
	struct gpu_data_type_e<uint32_t>
	{
		static const uint32_t value = 0x1405;	//GL_UNSIGNED_INT
	};

	template<>
	struct gpu_data_type_e<float32_t>
	{
		static const uint32_t value = 0x1406;	//GL_FLOAT
	};

	template<>
	struct gpu_data_type_e<float64_t>
	{
		static const uint32_t value = 0x140A;	//GL_DOUBLE
	};

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
}