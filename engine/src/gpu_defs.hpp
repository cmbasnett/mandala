#pragma once

//naga
#include "types.hpp"
#include "rectangle.hpp"

namespace naga
{
    namespace details
	{
		template<typename Value, Value Default>
        struct ValueWrapper
        {
            typedef Value ValueType;
			typedef ValueWrapper<ValueType, Default> Type;

			ValueWrapper() = default;
			ValueWrapper(const ValueType& value) :
                value(value)
            {
            }

			operator Value() const
            {
                return value;
            }

            Type& operator=(ValueType value)
            {
                this->value = value;

                return *this;
            }

			ValueType* operator&()
            {
                return &value;
            }

        private:
            ValueType value = Default;
        };
    }

    typedef details::ValueWrapper<u32, 0>	GpuId;
	typedef details::ValueWrapper<i32, -1>	GpuLocation;
	typedef details::ValueWrapper<u32, 0>	GpuIndex;

    typedef Rectangle		GpuViewportType;
    typedef u8              GpuClearFlagType;
    typedef vec2			GpuFrameBufferSizeType;
    typedef u8              GpuFrameBufferTypeFlagsType;

    enum : GpuFrameBufferTypeFlagsType
    {
        GPU_FRAME_BUFFER_TYPE_FLAG_COLOR = (1 << 0),
        GPU_FRAME_BUFFER_TYPE_FLAG_DEPTH = (1 << 1),
        GPU_FRAME_BUFFER_TYPE_FLAG_STENCIL = (1 << 2)
    };

    enum class GpuFrameBufferType : GpuFrameBufferTypeFlagsType
    {
        COLOR = (GPU_FRAME_BUFFER_TYPE_FLAG_COLOR),
        COLOR_DEPTH = (GPU_FRAME_BUFFER_TYPE_FLAG_COLOR | GPU_FRAME_BUFFER_TYPE_FLAG_DEPTH),
        COLOR_DEPTH_STENCIL = (GPU_FRAME_BUFFER_TYPE_FLAG_COLOR | GPU_FRAME_BUFFER_TYPE_FLAG_DEPTH | GPU_FRAME_BUFFER_TYPE_FLAG_STENCIL),
        DEPTH = (GPU_FRAME_BUFFER_TYPE_FLAG_DEPTH),
        DEPTH_STENCIL = (GPU_FRAME_BUFFER_TYPE_FLAG_DEPTH | GPU_FRAME_BUFFER_TYPE_FLAG_STENCIL)
    };

    enum class GpuDataTypes
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
    struct GpuDataType;

    template<>
    struct GpuDataType<i8>
    {
		static const auto VALUE = GpuDataTypes::BYTE;
    };

    template<>
    struct GpuDataType<u8>
    {
		static const auto VALUE = GpuDataTypes::UNSIGNED_BYTE;
    };

    template<>
    struct GpuDataType<i16>
    {
		static const auto VALUE = GpuDataTypes::SHORT;
    };

    template<>
    struct GpuDataType<u16>
    {
		static const auto VALUE = GpuDataTypes::UNSIGNED_SHORT;
    };

    template<>
    struct GpuDataType<i32>
    {
		static const auto VALUE = GpuDataTypes::INT;
    };

    template<>
    struct GpuDataType<u32>
    {
		static const auto VALUE = GpuDataTypes::UNSIGNED_INT;
    };

    template<>
    struct GpuDataType<f32>
    {
		static const auto VALUE = GpuDataTypes::FLOAT;
    };

    template<>
    struct GpuDataType<f64>
    {
		static const auto VALUE = GpuDataTypes::DOUBLE;
    };
}