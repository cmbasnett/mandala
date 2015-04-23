//mandala
#include "python_mgr.hpp"
#include "python_defs.hpp"

//boost
#include <boost\python.hpp>

namespace mandala
{
	python_mgr_t python;

	python_mgr_t::python_mgr_t()
	{
        using namespace boost;
        using namespace boost::python;

        PyImport_AppendInittab("mandala", &initmandala);

		Py_Initialize();

		main_module = object(handle<>(borrowed(PyImport_AddModule("__main__"))));
		main_namespace = main_module.attr("__dict__");
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