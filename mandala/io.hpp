#pragma once

//std
#include <ostream>

//glm
#include <glm\glm.hpp>

template<typename T>
inline void read(std::istream& istream, T& t)
{
	istream.read(reinterpret_cast<char*>(&t), sizeof(T));
}

template<typename T>
inline void write(std::ostream& ostream, T& t)
{
    ostream.write(reinterpret_cast<char*>(&t), sizeof(T));
}

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
