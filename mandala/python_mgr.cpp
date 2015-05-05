//mandala
#include "python_mgr.hpp"
#include "python_defs.hpp"

//boost
#include <boost\python.hpp>

namespace mandala
{
	python_mgr_t py;

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

    void python_mgr_t::exec_file(const std::string& file)
    {
        try
        {
            auto ignored = boost::python::exec_file(file.c_str(), main_namespace);
        }
        catch (boost::python::error_already_set const&)
        {
            std::string message;

            if (PyErr_Occurred())
            {
                message = handle_pyerr();

                std::cout << message;
            }

            boost::python::handle_exception();

            PyErr_Clear();

            throw std::exception(message.c_str());
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

    std::string python_mgr_t::handle_pyerr()
    {
        using namespace boost::python;
        using namespace boost;

        PyObject *exc, *val, *tb;
        object formatted_list, formatted;
        PyErr_Fetch(&exc, &val, &tb);
        handle<> hexc(exc), hval(allow_null(val)), htb(allow_null(tb));
        object traceback(import("traceback"));

        if (!tb)
        {
            object format_exception_only(traceback.attr("format_exception_only"));
            formatted_list = format_exception_only(hexc, hval);
        }
        else
        {
            object format_exception(traceback.attr("format_exception"));
            formatted_list = format_exception(hexc, hval, htb);
        }

        formatted = str("\n").join(formatted_list);

        return extract<std::string>(formatted);
    }
}