#pragma once

//boost
#include <boost\python.hpp>

//mandala
#include "app.hpp"
#include "string_mgr.hpp"
#include "state_mgr.hpp"
#include "resource_mgr.hpp"
#include "gui_label.hpp"
#include "bitmap_font.hpp"
#include "padding.hpp"
#include "io.hpp"
#include "state.hpp"
#include "input_event.hpp"
#include "gui_state.hpp"

using namespace boost;
using namespace boost::python;
using namespace mandala;

#define BOOST_DEFINE_VEC2_CLASS(name, value_type)\
class_<glm::detail::tvec2<value_type>>(name, init<value_type, value_type>())\
    .def_readwrite("x", &glm::detail::tvec2<value_type>::x)\
    .def_readwrite("y", &glm::detail::tvec2<value_type>::y)\
    .def(self_ns::self + self_ns::self)\
    .def(self_ns::self += self_ns::self)\
    .def(self_ns::self - self_ns::self)\
    .def(self_ns::self -= self_ns::self)\
    .def(self_ns::str(self_ns::self));\

#define BOOST_DEFINE_VEC3_CLASS(name, value_type)\
class_<glm::detail::tvec3<value_type>>(name, init<value_type, value_type, value_type>())\
    .def_readwrite("x", &glm::detail::tvec3<value_type>::x)\
    .def_readwrite("y", &glm::detail::tvec3<value_type>::y)\
    .def_readwrite("z", &glm::detail::tvec3<value_type>::z)\
    .def(self_ns::self + self_ns::self)\
    .def(self_ns::self += self_ns::self)\
    .def(self_ns::self - self_ns::self)\
    .def(self_ns::self -= self_ns::self)\
    .def(self_ns::str(self_ns::self));\

#define BOOST_DEFINE_VEC4_CLASS(name, value_type)\
class_<glm::detail::tvec4<value_type>>(name, init<value_type, value_type, value_type, value_type>())\
    .def_readwrite("x", &glm::detail::tvec4<value_type>::x)\
    .def_readwrite("y", &glm::detail::tvec4<value_type>::y)\
    .def_readwrite("z", &glm::detail::tvec4<value_type>::z)\
    .def_readwrite("w", &glm::detail::tvec4<value_type>::w)\
    .def(self_ns::self + self_ns::self)\
    .def(self_ns::self += self_ns::self)\
    .def(self_ns::self - self_ns::self)\
    .def(self_ns::self -= self_ns::self)\
    .def(self_ns::str(self_ns::self));\

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
    BOOST_DEFINE_VEC2_CLASS("Vec2f", float);
    BOOST_DEFINE_VEC2_CLASS("Vec2i", int);
    BOOST_DEFINE_VEC2_CLASS("Vec2d", double);

    BOOST_DEFINE_VEC3_CLASS("Vec3f", float);
    BOOST_DEFINE_VEC3_CLASS("Vec3i", int);
    BOOST_DEFINE_VEC3_CLASS("Vec3d", double);

    BOOST_DEFINE_VEC4_CLASS("Vec4f", float);
    BOOST_DEFINE_VEC4_CLASS("Vec4i", int);
    BOOST_DEFINE_VEC4_CLASS("Vec4d", double);

    class_<padding_f32_t>("Padding", init<float, float, float, float>())
        .def_readwrite("bottom", &padding_t::bottom)
        .def_readwrite("left", &padding_t::left)
        .def_readonly("top", &padding_t::top)
        .def_readonly("right", &padding_t::right)
        .add_property("vertical", &padding_t::vertical)
        .add_property("horizontal", &padding_t::horizontal)
        .add_property("size", &padding_t::size)
        //TODO: these don't compile for some reason
        //.def(self_ns::self + other<padding_f32_t>())
        //.def(self_ns::self += self_ns::self)
        //.def(self_ns::self - self_ns::self)
        //.def(self_ns::self -= self_ns::self)
        .def(self_ns::str(self_ns::self));

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
        .add_property("count", &state_mgr_t::count)
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

    scope().attr("STATE_FLAG_NONE") = state_flags_type(state_flag_none);
    scope().attr("STATE_FLAG_RENDER") = state_flags_type(state_flag_render);
    scope().attr("STATE_FLAG_INPUT") = state_flags_type(state_flag_input);
    scope().attr("STATE_FLAG_TICK") = state_flags_type(state_flag_tick);
    scope().attr("STATE_FLAG_ALL") = state_flags_type(state_flag_all);

    {
        scope gui_label_scope = class_<gui_label_t, bases<gui_node_t>, std::shared_ptr<gui_label_t>, noncopyable>("GuiLabel", init<>())
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

    class_<state_t, std::shared_ptr<state_t>, noncopyable>("State", init<>())
        .def("tick", &state_t::tick)
        .def("on_input_event", &state_t::on_input_event)
#if defined(MANDALA_PC)
        .def("on_window_event", &state_t::on_window_event)
#endif
        .def("on_active", &state_t::on_active)
        .def("on_passive", &state_t::on_passive)
        .def("on_enter", &state_t::on_enter)
        .def("on_exit", &state_t::on_exit)
        .def("on_stop_tick", &state_t::on_stop_tick)
        .def("on_start_tick", &state_t::on_start_tick)
        .def("on_stop_render", &state_t::on_stop_render)
        .def("on_start_render", &state_t::on_start_render)
        .def("on_stop_input", &state_t::on_stop_input)
        .def("on_start_input", &state_t::on_start_input);

    class_<gui_state_t, bases<state_t>, std::shared_ptr<gui_state_t>, noncopyable>("GuiState", init<>())
        .def_readonly("layout", &gui_state_t::layout);

    class_<gui_layout_t, bases<gui_node_t>, std::shared_ptr<gui_layout_t>, noncopyable>("GuiLayout", no_init);

    //TODO: remove specialized platform structs, have only 1 platform_t per platform
#if defined(MANDALA_WINDOWS)
    class_<platform_win32_t, noncopyable>("Platform", no_init)
#elif defined(MANDALA_OSX)
    class_<platform_osx_t, noncopyable>("Platform", no_init)
#endif
        .add_property("screen_size", &platform_t::get_screen_size, &platform_t::set_screen_size)
#if defined(MANDALA_PC)
        .add_property("cursor_location", &platform_t::get_cursor_location, &platform_t::set_cursor_location)
#endif
        ;

    {
        scope input_event_scope = class_<input_event_t, noncopyable>("InputEvent", no_init)
            .def_readonly("id", &input_event_t::id)
            .def_readonly("device_type", &input_event_t::device_type)
            .def_readonly("touch", &input_event_t::touch)
            .def_readonly("keyboard", &input_event_t::keyboard)
            //#if defined(MANDALA_PC)
            //            //.def_readonly("gamepad", &input_event_t::gamepad)
            //#endif
            .def_readonly("is_consumed", &input_event_t::is_consumed);

        enum_<input_event_t::device_type_e>("DeviceType")
            .value("NONE", input_event_t::device_type_e::none)
            .value("TOUCH", input_event_t::device_type_e::touch)
            .value("KEYBOARD", input_event_t::device_type_e::keyboard)
#if defined(MANDALA_PC)
            .value("GAMEPAD", input_event_t::device_type_e::gamepad)
#endif
            .value("COUNT", input_event_t::device_type_e::count)
            .export_values();

        scope().attr("MOD_FLAG_SHIFT") = input_event_t::mod_flags_type(input_event_t::mod_flag_shift);
        scope().attr("MOD_FLAG_CTRL") = input_event_t::mod_flags_type(input_event_t::mod_flag_ctrl);
        scope().attr("MOD_FLAG_ALT") = input_event_t::mod_flags_type(input_event_t::mod_flag_alt);
        scope().attr("MOD_FLAG_SUPER") = input_event_t::mod_flags_type(input_event_t::mod_flag_super);

        {
            scope touch_scope = class_<input_event_t::touch_t, noncopyable>("Touch", no_init)
                .def_readonly("id", &input_event_t::touch_t::id)
                .def_readonly("type", &input_event_t::touch_t::type)
#if defined(MANDALA_PC)
                .def_readonly("button", &input_event_t::touch_t::button)
#endif
                .def_readonly("location", &input_event_t::touch_t::location)
                .def_readonly("location_delta", &input_event_t::touch_t::location_delta)
                .def_readonly("mod_flags", &input_event_t::touch_t::mod_flags);

            enum_<input_event_t::touch_t::type_e>("Type")
                .value("NONE", input_event_t::touch_t::type_e::none)
                .value("PRESS", input_event_t::touch_t::type_e::press)
                .value("RELEASE", input_event_t::touch_t::type_e::release)
                .value("SCROLL", input_event_t::touch_t::type_e::scroll)
                .value("MOVE", input_event_t::touch_t::type_e::move)
                .export_values();

#if defined(MANDALA_PC)
            enum_<input_event_t::touch_t::button_e>("Button")
                .value("NONE", input_event_t::touch_t::button_e::none)
                .value("LEFT", input_event_t::touch_t::button_e::left)
                .value("RIGHT", input_event_t::touch_t::button_e::right)
                .value("MIDDLE", input_event_t::touch_t::button_e::middle)
                .value("FOUR", input_event_t::touch_t::button_e::four)
                .value("FIVE", input_event_t::touch_t::button_e::five)
                .value("SIX", input_event_t::touch_t::button_e::six)
                .value("SEVEN", input_event_t::touch_t::button_e::seven)
                .value("EIGHT", input_event_t::touch_t::button_e::eight)
                .value("COUNT", input_event_t::touch_t::button_e::count)
                .export_values();
#endif
        }

#if defined(MANDALA_PC)
        {
            scope keyboard_scope = class_<input_event_t::keyboard_t, noncopyable>("Keyboard", no_init)
                .def_readonly("type", &input_event_t::keyboard_t::type)
                //.def_readonly("key", &input_event_t::keyboard_t::key) //TODO: do this later
                .def_readonly("mod_flags", &input_event_t::keyboard_t::mod_flags)
                .def_readonly("character", &input_event_t::keyboard_t::character);

            enum_<input_event_t::keyboard_t::type_e>("Type")
                .value("NONE", input_event_t::keyboard_t::type_e::none)
                .value("KEY_RELEASE", input_event_t::keyboard_t::type_e::key_release)
                .value("KEY_PRESS", input_event_t::keyboard_t::type_e::key_press)
                .value("KEY_REPEAT", input_event_t::keyboard_t::type_e::key_repeat)
                .value("CHARACTER", input_event_t::keyboard_t::type_e::character)
                .export_values();
        }
#endif
    }

    scope().attr("app") = boost::ref(app);
    scope().attr("strings") = boost::ref(strings);
    scope().attr("states") = boost::ref(states);
    scope().attr("resources") = boost::ref(resources);
    scope().attr("platform") = boost::ref(platform);
}