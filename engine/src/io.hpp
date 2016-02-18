#pragma once

//std
#include <ostream>
#include <array>
#include <vector>

//glm
#include <glm\glm.hpp>

//naga
#include "types.hpp"
#include "range.hpp"

template<typename T>
inline void read(std::istream& istream, T& t)
{
    istream.read(reinterpret_cast<char*>(&t), sizeof(T));
}

template<typename T>
inline void read(std::istream& istream, std::string& s)
{
    std::getline(istream, s, '\0');
}

template<typename T, size_t N>
inline void read(std::istream& istream, std::array<T, N>& data)
{
    istream.read(reinterpret_cast<char*>(data.data()), sizeof(T) * N);
}

// TODO: not the most efficient thing in the world, ask on SO how this might be done better
// drawbacks:
// T must have a default constructor
template<typename T>
inline void read(std::istream& istream, std::vector<T>& data, size_t count)
{
    data.reserve(count);

    std::vector<T> buffer;
    buffer.resize(count);

    istream.read(reinterpret_cast<char*>(buffer.data()), sizeof(T) * count);

    std::copy(buffer.begin(), buffer.end(), std::back_inserter(data));
}

template<typename T, size_t N>
inline void read(std::istream& istream, T data[N])
{
    istream.read(reinterpret_cast<char*>(data), _countof(data) * sizeof(T)); //TODO: same as sizeof(data)?
}

template<typename T>
inline void write(std::ostream& ostream, T& data)
{
    ostream.write(reinterpret_cast<char*>(&data), sizeof(T));
}

template<typename T>
inline void write(std::ostream& ostream, const T* data, size_t count)
{
    ostream.write(unsafe_cast<char*>(data), count);
}

template<typename T>
inline void write(std::ostream& ostream, std::vector<T>& data)
{
    ostream.write(reinterpret_cast<char*>(data.data()), sizeof(T) * data.size());
}

namespace glm
{
    namespace detail
    {
        template<typename T>
        std::ostream& operator<<(std::ostream& ostream, const tvec2<T>& v)
        {
            return ostream << "(" << v.x << "," << v.y << ")";
        }

        template<typename T>
        std::ostream& operator<<(std::ostream& ostream, const tvec3<T>& v)
        {
            return ostream << "(" << v.x << "," << v.y << "," << v.z << ")";
        }

        template<typename T>
        std::ostream& operator<<(std::ostream& ostream, const tvec4<T>& v)
        {
            return ostream << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
        }

        template<typename T>
        std::ostream& operator<<(std::ostream& ostream, const tquat<T>& q)
        {
            return ostream << "(" << q.x << "," << q.y << "," << q.z << "," << q.w << ")";
        }
    }
}

namespace naga
{
    template<typename T>
    std::ostream& operator<<(std::ostream& ostream, const range_<T>& aabb)
    {
        return ostream << "(min=" << aabb.min << ",max=" << aabb.max << ")";
    }
}