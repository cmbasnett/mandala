#pragma once

//std
#include <exception>
#include <sstream>

namespace naga
{
    template<typename T>
    struct bad_value : std::exception
    {
        typedef T value_type;

        value_type expected;
        value_type actual;

        bad_value(const value_type& expected, const value_type& actual) :
            expected(expected),
            actual(actual)
        {
        }
    };

    struct bad_version : bad_value<unsigned int>
    {
        const char* what() const override
        {
            std::ostringstream oss;
            oss << "bad version (expected: " << expected << ", actual: " << actual << ")";
            return oss.str().c_str();
        }
    };

    struct bad_magic : bad_value<std::string>
    {
        const char* what() const override
        {
            std::ostringstream oss;
            oss << "bad magic (expected: " << expected << ", actual: " << actual << ")";
            return oss.str().c_str();
        }
    };
}