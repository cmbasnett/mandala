#pragma once

#include <boost\python.hpp>

using namespace boost::python;

namespace naga
{
	template<typename T, typename U>
	struct python_pair
	{
		typedef std::pair<T, U> pair_type;

		struct conversion : public boost::python::converter::expected_from_python_type<pair_type>
		{
			static PyObject* convert(const pair_type& pair)
			{
				auto tuple = boost::python::make_tuple(pair.first, pair.second);
				return incref(object(tuple).ptr());
			}
		};

		static void* convertible(PyObject *obj)
		{
			return (PyTuple_Check(obj) &&
				PyTuple_Size(obj) == 2 &&
				extract<T>(PyTuple_GetItem(obj, 0)).check() &&
				extract<U>(PyTuple_GetItem(obj, 1)).check()) ? obj : nullptr;
		}

		static void constructor(PyObject *obj, boost::python::converter::rvalue_from_python_stage1_data *data)
		{
			void* const storage = reinterpret_cast<converter::rvalue_from_python_storage<pair_type>*>(data)->storage.bytes;

			new (storage) pair_type(extract<T>(PyTuple_GetItem(obj, 0)), extract<T>(PyTuple_GetItem(obj, 1)));

			data->convertible = storage;
		}

		explicit python_pair()
		{
			if (!extract<pair_type>(object()).check())
			{
				to_python_converter<pair_type, conversion, true>();

				converter::registry::push_back(
					&convertible,
					&constructor,
					type_id<pair_type>(),
					&conversion::get_pytype);
			}
		}
	};
}