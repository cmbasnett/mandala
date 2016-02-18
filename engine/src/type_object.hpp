// boost
#include <boost\python\object.hpp>

/// @brief boost::python::object that refers to a type.
struct type_object : public boost::python::object
{
    /// @brief If the object is a type, then refer to it.  Otherwise, refer to the instance's type.
    explicit type_object(boost::python::object object) :
        boost::python::object(object)
    {
        if (!PyType_Check(object.ptr()))
        {
            throw std::invalid_argument("type_object requires a Python type");
        }
    }
};

/// @brief Enable automatic conversions to type_object.
struct enable_type_object
{
    enable_type_object()
    {
        boost::python::converter::registry::push_back(
            &convertible,
            &construct,
            boost::python::type_id<type_object>());
    }

    static void* convertible(PyObject* object)
    {
        return PyType_Check(object) ? object : nullptr;
    }

    static void construct(
        PyObject* object,
        boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        // Obtain a handle to the memory block that the converter has allocated
        // for the C++ type.
        namespace python = boost::python;

        typedef python::converter::rvalue_from_python_storage<type_object> storage_type;

        void* storage = reinterpret_cast<storage_type*>(data)->storage.bytes;

        // Construct the type object within the storage.  Object is a borrowed 
        // reference, so create a handle indicting it is borrowed for proper
        // reference counting.
        python::handle<> handle(python::borrowed(object));

        new (storage)type_object(python::object(handle));

        // Set convertible to indicate success. 
        data->convertible = storage;
    }
};
