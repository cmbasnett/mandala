//mandala
#include "python_mgr.hpp"
#include "app.hpp"
#include "string_mgr.hpp"
#include "state_mgr.hpp"
#include "resource_mgr.hpp"
#include "gui_label.hpp"

//boost
#include <boost\python.hpp>

using namespace boost;
using namespace boost::python;
using namespace mandala;

BOOST_PYTHON_MODULE(mandala)
{
    class_<app_t, noncopyable>("App", no_init)
        .def("exit", &app_t::exit)
        .def("reset", &app_t::reset);

    class_<string_mgr_t, noncopyable>("StringMgr", no_init)
        .def("mount", &string_mgr_t::mount)
        .def("get", &string_mgr_t::get)
        .def("purge", &string_mgr_t::purge);

    class_<state_mgr_t, noncopyable>("StateMgr", no_init)
        .def("push", &state_mgr_t::push)
        .def("pop", &state_mgr_t::pop)
        .def("change_link_flags", &state_mgr_t::change_link_flags)
        .def("purge", &state_mgr_t::purge)
        .def("get_flags", &state_mgr_t::get_flags)
        .def("get_link_flags", &state_mgr_t::get_link_flags)
        .def("count", &state_mgr_t::count)
        .def("at", &state_mgr_t::at)
        .def("index_of", &state_mgr_t::index_of)
        .def("is_state_rendering", &state_mgr_t::is_state_rendering)
        .def("is_state_ticking", &state_mgr_t::is_state_ticking)
        .def("is_state_handling_input", &state_mgr_t::is_state_handling_input)
        .def("is_state_popping", &state_mgr_t::is_state_popping)
        .def("is_state_changing_link_flags", &state_mgr_t::is_state_changing_link_flags);

    class_<resource_mgr_t, noncopyable>("ResourceMgr", no_init)
        .def("mount", &resource_mgr_t::mount)
        .def("unmount_all", &resource_mgr_t::unmount_all)
        .def("extract", &resource_mgr_t::extract)
        .def("count", &resource_mgr_t::count)
        .def("prune", &resource_mgr_t::prune)
        .def("purge", &resource_mgr_t::purge);

    class_<gui_label_t, std::shared_ptr<gui_label_t>, noncopyable>("GuiLabel", init<>());
        //.add_property("string", &gui_label_t::get_string, &gui_label_t::set_string);

    scope().attr("app") = boost::ref(app);
    scope().attr("strings") = boost::ref(strings);
    scope().attr("states") = boost::ref(states);
    scope().attr("resources") = boost::ref(resources);
}

namespace mandala
{
	python_mgr_t python;

	python_mgr_t::python_mgr_t()
	{
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