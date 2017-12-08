#pragma once

//std
#include <chrono>
#include <type_traits>
#include <string>

namespace naga
{
    struct Resource
    {
		typedef std::chrono::system_clock ClockType;
		typedef ClockType::time_point TimePointType;

		std::string name;
		TimePointType last_access_time;

		const TimePointType& get_creation_time() const { return creation_time; }

    protected:
		Resource();

    private:
		TimePointType creation_time;

		Resource(const Resource&) = delete;
		Resource& operator=(const Resource&) = delete;
    };

    template<typename T, typename Enable = void>
    struct IsResource : std::false_type { };

    template<typename T>
	struct IsResource<T, typename std::enable_if<std::is_base_of<Resource, T>::value>::type> : std::true_type{};
}
