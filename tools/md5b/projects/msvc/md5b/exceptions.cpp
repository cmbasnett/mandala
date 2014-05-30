//md5b
#include <sstream>

//md5b
#include "exceptions.h"

namespace md5b
{
    bad_version::bad_version(size_t expected, size_t actual) :
        expected(expected),
        actual(actual)
    {
    }

    const char* bad_version::what() const
    {
        std::ostringstream oss;
        oss << "bad version (expected " << expected << ", actual " << actual << ")";
        return oss.str().c_str();
    }

    const char* bad_format::what() const
    {
        return "bad format";
    }
};
