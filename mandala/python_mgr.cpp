//mandala
#include "python_mgr.hpp"
#include "app.hpp"

//boost
#include <boost\python.hpp>

//TODO: remove later, only needed for building python modules
#include <iostream> 

using namespace boost;
using namespace boost::python;

void say_hello(const char* name) {
	std::cout << "Hello " << name << "!" << std::endl;
}

BOOST_PYTHON_MODULE(hello)
{
	def("say_hello", say_hello);
};

namespace mandala
{
	python_mgr_t python;

	python_mgr_t::python_mgr_t()
	{
		using namespace boost::python;

		Py_Initialize();

		main_module = object(handle<>(borrowed(PyImport_AddModule("__main__"))));
		main_namespace = main_module.attr("__dict__");

        auto hello_module = object(handle<>(borrowed(PyImport_ImportModule("hello"))));
        main_namespace["hello"] = hello_module;
	}

	void python_mgr_t::exec(const std::string& string)
	{
		try
		{
			auto ignored = boost::python::exec(string.c_str(), main_namespace);
		}
		catch (boost::python::error_already_set const&)
		{
			PyObject* type = nullptr;
			PyObject* value = nullptr;
			PyObject* traceback = nullptr;

			PyErr_Fetch(&type, &value, &traceback);

			auto error_string = PyString_AsString(value);

			PyErr_Clear();

			throw std::exception(error_string);
		}
	}
}