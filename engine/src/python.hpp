#pragma once

//std
#include <string>

#ifndef BOOST_PYTHON_STATIC_LIB
#define BOOST_PYTHON_STATIC_LIB
#endif

//boost
#include <boost\python.hpp>

//naga
#include "api.hpp"

namespace naga
{
    struct Python
    {
		Python();
		~Python();

        boost::python::api::object exec(const char* string);
        boost::python::api::object NAGA_API_ENTRY exec_file(const char* file);
        boost::python::api::object eval(const char* string);

        void finalize();

        static std::string handle_pyerr();

    private:
        boost::python::object main_namespace;
        boost::python::object main_module;
    };

	NAGA_API_ENTRY extern Python py;
}
