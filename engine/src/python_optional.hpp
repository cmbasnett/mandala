#pragma once

//boost
#include <boost\optional.hpp>
#include <boost\noncopyable.hpp>
#include <boost\python.hpp>

template<typename T>
struct python_optional : private boost::noncopyable
{
    struct conversion : public boost::python::converter::expected_from_python_type<T>
    {
        static PyObject* convert(boost::optional<T> const& value)
        {
            return incref((value ? object(*value) : object()).ptr());
        }
    };

    static void* convertible(PyObject *obj)
    {
        return obj == Py_None || extract<T>(obj).check() ? obj : nullptr;
    }

    static void constructor(PyObject *obj, boost::python::converter::rvalue_from_python_stage1_data *data)
    {
        void *const storage = reinterpret_cast<converter::rvalue_from_python_storage<boost::optional<T>>*>(data)->storage.bytes;

        if (obj == Py_None)
        {
            new (storage) boost::optional<T>();
        }
        else
        {
            new (storage) boost::optional<T>(extract<T>(obj));
        }

        data->convertible = storage;
    }

    explicit python_optional()
    {
        if (!extract<boost::optional<T>>(object()).check())
        {
            to_python_converter<boost::optional<T>, conversion, true>();

            converter::registry::push_back(
                &convertible,
                &constructor,
                type_id<boost::optional<T>>(),
                &conversion::get_pytype);
        }
    }
};