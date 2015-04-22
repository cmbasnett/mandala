//mandala
#include "python_mgr.hpp"
#include "app.hpp"
#include "string_mgr.hpp"
#include "state_mgr.hpp"
#include "resource_mgr.hpp"
#include "gui_label.hpp"
#include "bitmap_font.hpp"
#include "padding.hpp"
#include "io.hpp"

//boost
#include <boost\python.hpp>

using namespace boost;
using namespace boost::python;
using namespace mandala;

BOOST_PYTHON_MODULE(mandala)
{
    class_<hash_t>("Hash", init<std::string>());

    class_<aabb2_t>("AABB2")
        .def_readwrite("min", &aabb2_t::min)
        .def_readwrite("max", &aabb2_t::max)
        .add_property("width", &aabb2_t::width)
        .add_property("height", &aabb2_t::height)
        .add_property("size", &aabb2_t::size)
        .add_property("center", &aabb2_t::center);

    //move to GLM module?
    class_<vec2_t>("Vec2", init<float, float>())
        .def_readwrite("x", &vec2_t::x)
        .def_readwrite("y", &vec2_t::y)
        .def(self_ns::self + self_ns::self)
        .def(self_ns::self += self_ns::self)
        .def(self_ns::self - self_ns::self)
        .def(self_ns::self -= self_ns::self)
        .def(self_ns::str(self_ns::self))
        ;

    class_<padding_f32_t>("Padding", init<float, float, float, float>())
        .def_readwrite("bottom", &padding_t::bottom)
        .def_readwrite("left", &padding_t::left)
        .def_readonly("top", &padding_t::top)
        .def_readonly("right", &padding_t::right)
        .add_property("vertical", &padding_t::vertical)
        .add_property("horizontal", &padding_t::horizontal)
        .add_property("size", &padding_t::size);

    class_<app_t, noncopyable>("App", no_init)
        .def("exit", &app_t::exit)
        .def("reset", &app_t::reset);

    class_<string_mgr_t, noncopyable>("StringMgr", no_init)
        .def("mount", &string_mgr_t::mount)
        .def("get", &string_mgr_t::get)
        .def("purge", &string_mgr_t::purge)
        .add_property("count", &string_mgr_t::count);

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

    class_<bitmap_font_t, std::shared_ptr<bitmap_font_t>, noncopyable>("BitmapFont", no_init);

    class_<gui_node_t, std::shared_ptr<gui_node_t>, noncopyable>("GuiNode", init<>())
        .add_property("parent", make_function(&gui_node_t::get_parent, return_value_policy<copy_const_reference>()))
        .add_property("dock_mode", &gui_node_t::get_dock_mode, &gui_node_t::set_dock_mode)
        .add_property("anchor_flags", &gui_node_t::get_anchor_flags, &gui_node_t::set_anchor_flags)
        //.add_property("anchor_offset", &gui_node_t::get_anchor_offset, &gui_node_t::set_anchor_offset)
        //.add_property("padding", &gui_node_t::get_padding, &gui_node_t::set_padding)
        //.add_property("margin", &gui_node_t::get_margin, &gui_node_t::set_margin)
        ////.add_property("size_mode", )
        //.add_property("size", &gui_node_t::get_size, &gui_node_t::set_size)
        //.add_property("color", &gui_node_t::get_color, &gui_node_t::set_color)
        //.add_property("bounds", &gui_node_t::get_bounds, &gui_node_t::set_bounds)
        .add_property("is_dirty", &gui_node_t::get_is_dirty)
        .add_property("is_hidden", &gui_node_t::get_is_hidden, &gui_node_t::set_is_hidden)
        .add_property("has_children", &gui_node_t::has_children)
        .add_property("has_parent", &gui_node_t::has_parent)
        .def("orphan", &gui_node_t::orphan)
        .def("dirty", &gui_node_t::dirty)
        .def("adopt", &gui_node_t::adopt)
        ;

    enum_<gui_dock_mode_e>("GuiDockMode")
        .value("NONE", gui_dock_mode_e::none)
        .value("BOTTOM", gui_dock_mode_e::bottom)
        .value("FILL", gui_dock_mode_e::fill)
        .value("LEFT", gui_dock_mode_e::left)
        .value("RIGHT", gui_dock_mode_e::right)
        .value("TOP", gui_dock_mode_e::top)
        .export_values();

    scope().attr("GUI_ANCHOR_FLAG_NONE") = gui_anchor_flags_type(gui_anchor_flag_none);
    scope().attr("GUI_ANCHOR_FLAG_BOTTOM") = gui_anchor_flags_type(gui_anchor_flag_bottom);
    scope().attr("GUI_ANCHOR_FLAG_LEFT") = gui_anchor_flags_type(gui_anchor_flag_left);
    scope().attr("GUI_ANCHOR_FLAG_RIGHT") = gui_anchor_flags_type(gui_anchor_flag_right);
    scope().attr("GUI_ANCHOR_FLAG_TOP") = gui_anchor_flags_type(gui_anchor_flag_top);
    scope().attr("GUI_ANCHOR_FLAG_VERTICAL") = gui_anchor_flags_type(gui_anchor_flag_vertical);
    scope().attr("GUI_ANCHOR_FLAG_HORIZONTAL") = gui_anchor_flags_type(gui_anchor_flag_horizontal);
    scope().attr("GUI_ANCHOR_FLAG_ALL") = gui_anchor_flags_type(gui_anchor_flag_all);

    {
        scope scope = class_<gui_label_t, bases<gui_node_t>, std::shared_ptr<gui_label_t>, noncopyable>("GuiLabel", init<>())
            /*.add_property("bitmap_font", &gui_label_t::get_bitmap_font, &gui_label_t::set_bitmap_font)
            .add_property("string", &gui_label_t::get_string, &gui_label_t::set_string)*/
            .add_property("justification", &gui_label_t::get_justification, &gui_label_t::set_justification)
            .add_property("vertical_alignment", &gui_label_t::get_vertical_alignment, &gui_label_t::set_vertical_alignment)
            .add_property("line_spacing", &gui_label_t::get_line_spacing, &gui_label_t::set_line_spacing)
            .add_property("is_multiline", &gui_label_t::get_is_multiline, &gui_label_t::set_is_multiline)
            .add_property("should_use_ellipses", &gui_label_t::get_should_use_ellipses, &gui_label_t::set_should_use_ellipses)
            .add_property("should_use_color_codes", &gui_label_t::get_should_use_color_codes, &gui_label_t::set_should_use_color_codes)
            .add_property("is_read_only", &gui_label_t::get_is_read_only, &gui_label_t::set_is_read_only);

        enum_<gui_label_t::justification_e>("Justification")
            .value("LEFT", gui_label_t::justification_e::left)
            .value("CENTER", gui_label_t::justification_e::center)
            .value("RIGHT", gui_label_t::justification_e::right)
            .export_values();

        enum_ <gui_label_t::vertical_alignment_e>("VerticalAlignment")
            .value("TOP", gui_label_t::vertical_alignment_e::top)
            .value("MIDDLE", gui_label_t::vertical_alignment_e::middle)
            .value("BOTTOM", gui_label_t::vertical_alignment_e::bottom)
            .export_values();
    }

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