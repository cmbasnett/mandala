#pragma once

namespace mandala
{
	template<typename T>
	struct range_
	{
        typedef T value_type;

        range_() = default;
        range_(const value_type& min, const value_type& max) :
            min(min),
            max(max)
        {
        }

        value_type min;
        value_type max;
	};
}
