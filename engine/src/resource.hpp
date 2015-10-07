#pragma once

//std
#include <chrono>
#include <type_traits>

//mandala
#include "hash.hpp"

namespace mandala
{
    struct resource
    {
        typedef std::chrono::system_clock clock_type;
        typedef clock_type::time_point time_point_type;

        hash hash;
        time_point_type last_access_time;

        const time_point_type& get_creation_time() const { return creation_time; }

    protected:
        resource();

    private:
        time_point_type creation_time;

        resource(const resource&) = delete;
        resource& operator=(const resource&) = delete;
    };

    template<typename T, typename Enable = void>
    struct is_resource : std::false_type { };

    template<typename T>
    struct is_resource<T, typename std::enable_if<std::is_base_of<resource, T>::value>::type> : std::true_type { };
}
