//boost
#include <boost\python.hpp>

//naga
#include "python.hpp"
#include "python_defs.hpp"

namespace naga
{
	NAGA_API_ENTRY Python py;

	Python::Python()
    {
        PyImport_AppendInittab("naga", &initnaga);

        Py_Initialize();

        main_module = object(handle<>(borrowed(PyImport_AddModule("__main__"))));
        main_namespace = main_module.attr("__dict__");
    }

	Python::~Python()
    {
    }

	void Python::finalize()
    {
        main_module = boost::python::object();
        main_namespace = boost::python::object();

        Py_Finalize();
    }

	boost::python::api::object Python::exec(const char* string)
    {
        try
        {
            return boost::python::exec(string, main_namespace);
        }
        catch (boost::python::error_already_set const&)
        {
            PyObject* type = nullptr;
            PyObject* value = nullptr;
            PyObject* traceback = nullptr;
            const char* error_string = nullptr;

            PyErr_Fetch(&type, &value, &traceback);
            PyErr_NormalizeException(&type, &value, &traceback);
            
            auto value_as_string = PyString_AsString(PyObject_Str(value));

            if (value_as_string)
            {
                error_string = value_as_string;
            }

            PyErr_Clear();

            throw std::exception(error_string);
        }
    }

	boost::python::api::object NAGA_API_ENTRY Python::exec_file(const char* file)
    {
        try
        {
            return boost::python::exec_file(file, main_namespace);
        }
        catch (const boost::python::error_already_set&)
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

	boost::python::api::object Python::eval(const char* string)
    {
        try
        {
            return boost::python::eval(string, main_namespace);
        }
        catch (boost::python::error_already_set const&)
        {
            PyObject* type = nullptr;
            PyObject* value = nullptr;
            PyObject* traceback = nullptr;
            const char* error_string = nullptr;

            PyErr_Fetch(&type, &value, &traceback);

            auto value_as_string = PyString_AsString(PyObject_Str(value));

            if (value_as_string)
            {
                error_string = value_as_string;
            }

            PyErr_Clear();

            throw std::exception(error_string);
        }
    }

	std::string Python::handle_pyerr()
    {
        PyObject* exc;
        PyObject* val;
        PyObject* tb;

        PyErr_Fetch(&exc, &val, &tb);

        handle<> hexc(exc);
        handle<> hval(allow_null(val));
        handle<> htb(allow_null(tb));

        object traceback(import("traceback"));
        object formatted_list;

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

        object formatted = str("\n").join(formatted_list);

        return extract<std::string>(formatted);
    }
}