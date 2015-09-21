#pragma once

//std
#include <string>

#ifndef BOOST_PYTHON_STATIC_LIB
#define BOOST_PYTHON_STATIC_LIB
#endif

//boost
#include <boost\python.hpp>

//mandala
#include "api.hpp"

namespace mandala
{
    struct python_t
    {
        python_t();
        ~python_t();

        boost::python::api::object exec(const char* string);
        boost::python::api::object MANDALA_API_ENTRY exec_file(const char* file);
        boost::python::api::object eval(const char* string);

        void finalize();

        static std::string handle_pyerr();

    private:
        boost::python::object main_namespace;
        boost::python::object main_module;
    };

    MANDALA_API_ENTRY extern python_t py;
}
