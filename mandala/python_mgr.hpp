#pragma once

//std
#include <string>

//boost
#include <boost\python.hpp>

namespace mandala
{
	struct python_mgr_t
	{
        python_mgr_t();

		void exec(const std::string& string);
        void exec_file(const std::string& file);

        //TODO: breaks RAII, figure out a way around this
        void initialize();
        void finalize();

		boost::python::object main_namespace;
		boost::python::object main_module;

    private:
        std::string handle_pyerr();
	};

	extern python_mgr_t py;
}
