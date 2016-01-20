#pragma once

#include <vector>

namespace naga
{
    template<typename T, typename V>
    size_t bisect_left(const std::vector<T>& a, V x, size_t start, size_t end)
    {
        if (start >= end)
        {
            return start;
        }

        if (end - start == 1)
        {
            if (a[end] > x)
            {
                return start;
            }
            else
            {
                return end;
            }
        }

        const auto i = start + ((end - start) / 2);

        if (a[i] > x)
        {
            return bisect_left(a, x, start, i);
        }
        else if (a[i] < x)
        {
            return bisect_left(a, x, i, end);
        }
        else
        {
            return i;
        }
    }
}