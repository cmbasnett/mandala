#pragma once

namespace naga
{
    template<typename T>
    struct Range
    {
        typedef T ValueType;

		Range() = default;
		Range(const ValueType& min, const ValueType& max) :
            min(min),
            max(max)
        {
        }

		ValueType min;
		ValueType max;
    };
}
