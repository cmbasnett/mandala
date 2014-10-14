#pragma once

//std
#include <ostream>

//glm
#include <glm\glm.hpp>

template<typename T>
std::ostream& operator<<(std::ostream& ostream, const glm::detail::tvec2<T>& v)
{
    return ostream << "[" << v.x << "," << v.y << "]";
}

template<typename T>
std::ostream& operator<<(std::ostream& ostream, const glm::detail::tvec3<T>& v)
{
    return ostream << "[" << v.x << "," << v.y << "," << v.z << "]";
}

template<typename T>
std::ostream& operator<<(std::ostream& ostream, const glm::detail::tvec4<T>& v)
{
    return ostream << "[" << v.x << "," << v.y << "," << v.z << "," << v.w << "]";
}
