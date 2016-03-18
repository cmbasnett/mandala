#pragma once

// boost
#include <boost\function.hpp>
#include <boost\python.hpp>

namespace naga
{
	template<typename F>
	struct python_function_from_callable
	{
		python_function_from_callable()
		{
			boost::python::converter::registry::push_back(&convertible, &construct, boost::python::type_id<boost::function<F>>());
		}

		static void* convertible(PyObject* obj)
		{
			return PyCallable_Check(obj) ? obj : nullptr;
		}

		static void construct(PyObject* obj, boost::python::converter::rvalue_from_python_stage1_data* data)
		{
			boost::python::object callable(boost::python::handle<>(boost::python::borrowed(obj)));
			void* storage = ((boost::python::converter::rvalue_from_python_storage< boost::function<F>>*) data)->storage.bytes;
			new (storage) boost::function<F>(callable);
			data->convertible = storage;
		}
	};
}
