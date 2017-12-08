#pragma once

//std
#include <exception>
#include <sstream>

namespace naga
{
	template<typename ValueType>
    struct BadValue : std::exception
    {
		ValueType expected;
		ValueType actual;

		BadValue(const ValueType& expected, const ValueType& actual) :
            expected(expected),
            actual(actual)
        {
        }
    };

	struct BadVersion : BadValue<unsigned int>
    {
        const char* what() const override
        {
            std::ostringstream oss;
            oss << "bad version (expected: " << expected << ", actual: " << actual << ")";
            return oss.str().c_str();
        }
    };

	struct BadMagic : BadValue<std::string>
    {
        const char* what() const override
        {
            std::ostringstream oss;
            oss << "bad magic (expected: " << expected << ", actual: " << actual << ")";
            return oss.str().c_str();
        }
    };
}