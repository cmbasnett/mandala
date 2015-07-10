#pragma once

//std
#include <string>

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

    private:
        boost::python::object main_namespace;
        boost::python::object main_module;

        std::string handle_pyerr();
    };

    MANDALA_API_ENTRY extern python_t py;
}
