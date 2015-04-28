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
        PyImport_AppendInittab("mandala", &initmandala);
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

            std::string error_string;
            
            auto value_as_string = PyString_AsString(value);

            if (value_as_string)
            {
                error_string = value_as_string;
            }

			PyErr_Clear();

			throw std::exception(error_string.c_str());
		}
	}

    void python_mgr_t::initialize()
    {
        using namespace boost;
        using namespace boost::python;

        Py_Initialize();

        main_module = object(handle<>(borrowed(PyImport_AddModule("__main__"))));
        main_namespace = main_module.attr("__dict__");
    }

    void python_mgr_t::finalize()
    {
        Py_Finalize();
    }
}