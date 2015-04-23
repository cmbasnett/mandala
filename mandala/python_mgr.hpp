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

		void exec(const std::string&);

        void purge();

		boost::python::object main_namespace;
		boost::python::object main_module;
	};

	extern python_mgr_t python;
}
