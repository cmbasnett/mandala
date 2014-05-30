#pragma once

#include <exception>

namespace md5b
{
    struct bad_version : std::exception
    {
        bad_version(size_t expected, size_t actual);

        virtual const char* what() const override;

        size_t expected;
        size_t actual;
    };

    struct bad_format : std::exception
    {
        virtual const char* what() const override;
    };
};
