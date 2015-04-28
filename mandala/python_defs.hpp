#pragma once

//boost
#include <boost\preprocessor.hpp>
#include <boost/python.hpp>

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
#include "gui_image.hpp"
#include "gui_button.hpp"
#include "rectangle.hpp"
#include "python_optional.hpp"
#include "texture.hpp"
#include "bsp.hpp"
#include "material.hpp"
#include "sprite_set.hpp"
#include "sound.hpp"
#include "image.hpp"
#include "model_info.hpp"   //TODO: ditch "info", convert to model & instance naming
#include "frame_buffer.hpp"
#include "gui_canvas.hpp"

using namespace boost;
using namespace boost::python;
using namespace mandala;

#define MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(return_type, name, ...)\
return_type name(__VA_ARGS__) override\
{\
    auto o = get_override(BOOST_PP_STRINGIZE(name));\
    \
    if (o)\
    {\
        o();\
    }\
    else\
    {\
        _wrapper_wrapped_type_::name();\
    }\
}\

struct state_wrapper_t : state_t, wrapper<state_t>
{
    void tick(float32_t dt) override
    {
        auto override_ = get_override("tick");

        if (override_)
        {
            override_(dt);
        }
        else
        {

            state_t::tick(dt);
        }
    }

    void on_input_event(input_event_t& input_event) override
    {
        auto override_ = get_override("on_input_event");

        if (override_)
        {
            override_(input_event);
        }
        else
        {
            state_t::on_input_event(input_event);
        }
    }

    //MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, tick, float32_t)
    //MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_input_event, input_event_t&)
    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_active)
    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_passive)
    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_enter)
    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_exit)
    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_stop_tick)
    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_start_tick)
    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_stop_render)
    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_start_render)
    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_stop_input)
    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_start_input)
};

#define MANDALA_DEFINE_RESOURCE_GET_FUNCTION(name)\
boost::shared_ptr<name##_t> resources_get_##name(resource_mgr_t& resources, const std::string& hash) { return resources.get<name##_t>(hash_t(hash)); }

MANDALA_DEFINE_RESOURCE_GET_FUNCTION(bitmap_font)
MANDALA_DEFINE_RESOURCE_GET_FUNCTION(bsp)
MANDALA_DEFINE_RESOURCE_GET_FUNCTION(image)
MANDALA_DEFINE_RESOURCE_GET_FUNCTION(material)
MANDALA_DEFINE_RESOURCE_GET_FUNCTION(model_info)
MANDALA_DEFINE_RESOURCE_GET_FUNCTION(sound)
MANDALA_DEFINE_RESOURCE_GET_FUNCTION(sprite_set)
MANDALA_DEFINE_RESOURCE_GET_FUNCTION(texture)

#define MANDALA_PYTHON_DEFINE_VEC2(name, value_type)\
class_<glm::detail::tvec2<value_type>>(name, init<value_type, value_type>())\
    .def_readwrite("x", &glm::detail::tvec2<value_type>::x)\
    .def_readwrite("y", &glm::detail::tvec2<value_type>::y)\
    .def(self_ns::self + self_ns::self)\
    .def(self_ns::self += self_ns::self)\
    .def(self_ns::self - self_ns::self)\
    .def(self_ns::self -= self_ns::self)\
    .def(self_ns::str(self_ns::self));\

#define MANDALA_PYTHON_DEFINE_VEC3(name, value_type)\
class_<glm::detail::tvec3<value_type>>(name, init<value_type, value_type, value_type>())\
    .def_readwrite("x", &glm::detail::tvec3<value_type>::x)\
    .def_readwrite("y", &glm::detail::tvec3<value_type>::y)\
    .def_readwrite("z", &glm::detail::tvec3<value_type>::z)\
    .def(self_ns::self + self_ns::self)\
    .def(self_ns::self += self_ns::self)\
    .def(self_ns::self - self_ns::self)\
    .def(self_ns::self -= self_ns::self)\
    .def(self_ns::str(self_ns::self));\

#define MANDALA_PYTHON_DEFINE_VEC4(name, value_type)\
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
    python_optional<sprite_t>();

    class_<hash_t>("Hash", init<std::string>());

    class_<aabb2_t>("AABB2")
        .def_readwrite("min", &aabb2_t::min)
        .def_readwrite("max", &aabb2_t::max)
        .add_property("width", &aabb2_t::width)
        .add_property("height", &aabb2_t::height)
        .add_property("size", &aabb2_t::size)
        .add_property("center", &aabb2_t::center);

    //move to GLM module?
    MANDALA_PYTHON_DEFINE_VEC2("Vec2f", float);
    MANDALA_PYTHON_DEFINE_VEC2("Vec2i", int);
    MANDALA_PYTHON_DEFINE_VEC2("Vec2u", unsigned int);
    MANDALA_PYTHON_DEFINE_VEC2("Vec2d", double);

    MANDALA_PYTHON_DEFINE_VEC3("Vec3f", float);
    MANDALA_PYTHON_DEFINE_VEC3("Vec3i", int);
    MANDALA_PYTHON_DEFINE_VEC3("Vec3f", double);

    MANDALA_PYTHON_DEFINE_VEC4("Vec4f", float);
    MANDALA_PYTHON_DEFINE_VEC4("Vec4i", int);
    MANDALA_PYTHON_DEFINE_VEC4("Vec4d", double);

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

    //PLATFORM
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
            .add_property("id", &input_event_t::id)
            .add_property("device_type", &input_event_t::device_type)
            .add_property("touch", &input_event_t::touch)
            .add_property("keyboard", &input_event_t::keyboard)
            //#if defined(MANDALA_PC)
            //            .def_readonly("gamepad", &input_event_t::gamepad)
            //#endif
            .add_property("is_consumed", &input_event_t::is_consumed);

        scope().attr("MOD_FLAG_SHIFT") = input_event_t::mod_flags_type(input_event_t::mod_flag_shift);
        scope().attr("MOD_FLAG_CTRL") = input_event_t::mod_flags_type(input_event_t::mod_flag_ctrl);
        scope().attr("MOD_FLAG_ALT") = input_event_t::mod_flags_type(input_event_t::mod_flag_alt);
        scope().attr("MOD_FLAG_SUPER") = input_event_t::mod_flags_type(input_event_t::mod_flag_super);

        enum_<input_event_t::device_type_e>("DeviceType")
            .value("NONE", input_event_t::device_type_e::none)
            .value("TOUCH", input_event_t::device_type_e::touch)
            .value("KEYBOARD", input_event_t::device_type_e::keyboard)
#if defined(MANDALA_PC)
            .value("GAMEPAD", input_event_t::device_type_e::gamepad)
#endif
            .value("COUNT", input_event_t::device_type_e::count)
            .export_values();

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
    scope().attr("platform") = boost::ref(platform);

    //APP
    class_<app_t, noncopyable>("App", no_init)
        .def("exit", &app_t::exit)
        .def("reset", &app_t::reset);

    scope().attr("app") = boost::ref(app);

    //STRING MANAGER
    class_<string_mgr_t, noncopyable>("StringMgr", no_init)
        .def("mount", &string_mgr_t::mount)
        .def("get", &string_mgr_t::get)
        .def("purge", &string_mgr_t::purge)
        .add_property("count", &string_mgr_t::count);

    scope().attr("strings") = boost::ref(strings);

    //STATE MANAGER
    scope().attr("STATE_FLAG_NONE") = state_flags_type(state_flag_none);
    scope().attr("STATE_FLAG_RENDER") = state_flags_type(state_flag_render);
    scope().attr("STATE_FLAG_INPUT") = state_flags_type(state_flag_input);
    scope().attr("STATE_FLAG_TICK") = state_flags_type(state_flag_tick);
    scope().attr("STATE_FLAG_ALL") = state_flags_type(state_flag_all);

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

    scope().attr("states") = boost::ref(states);

    //RESOURCES
    class_<resource_mgr_t, noncopyable>("ResourceMgr", no_init)
        .def("mount", &resource_mgr_t::mount)
        .def("unmount_all", &resource_mgr_t::unmount_all)
        .def("extract", &resource_mgr_t::extract)
        .add_property("count", &resource_mgr_t::count)
        .def("purge", &resource_mgr_t::purge)
        .def("get_bitmap_font", &resources_get_bitmap_font)
        .def("get_bsp", &resources_get_bsp)
        .def("get_image", &resources_get_image)
        .def("get_material", &resources_get_material)
        .def("get_model", &resources_get_model_info)
        .def("get_sound", &resources_get_sound)
        .def("get_sprite_set", &resources_get_sprite_set)
        .def("get_texture", &resources_get_texture)
        ;

    scope().attr("resources") = boost::ref(resources);

    //GUI
    scope().attr("GUI_ANCHOR_FLAG_NONE") = gui_anchor_flags_type(gui_anchor_flag_none);
    scope().attr("GUI_ANCHOR_FLAG_BOTTOM") = gui_anchor_flags_type(gui_anchor_flag_bottom);
    scope().attr("GUI_ANCHOR_FLAG_LEFT") = gui_anchor_flags_type(gui_anchor_flag_left);
    scope().attr("GUI_ANCHOR_FLAG_RIGHT") = gui_anchor_flags_type(gui_anchor_flag_right);
    scope().attr("GUI_ANCHOR_FLAG_TOP") = gui_anchor_flags_type(gui_anchor_flag_top);
    scope().attr("GUI_ANCHOR_FLAG_VERTICAL") = gui_anchor_flags_type(gui_anchor_flag_vertical);
    scope().attr("GUI_ANCHOR_FLAG_HORIZONTAL") = gui_anchor_flags_type(gui_anchor_flag_horizontal);
    scope().attr("GUI_ANCHOR_FLAG_ALL") = gui_anchor_flags_type(gui_anchor_flag_all);

    enum_<gui_size_mode_e>("GuiSizeMode")
        .value("ABSOLUTE", gui_size_mode_e::absolute)
        .value("INHERIT", gui_size_mode_e::inherit)
        .value("RELATIVE", gui_size_mode_e::relative)
        .export_values();

    enum_<gui_dock_mode_e>("GuiDockMode")
        .value("NONE", gui_dock_mode_e::none)
        .value("BOTTOM", gui_dock_mode_e::bottom)
        .value("FILL", gui_dock_mode_e::fill)
        .value("LEFT", gui_dock_mode_e::left)
        .value("RIGHT", gui_dock_mode_e::right)
        .value("TOP", gui_dock_mode_e::top)
        .export_values();

    class_<gui_node_t, boost::shared_ptr<gui_node_t>, noncopyable>("GuiNode", init<>())
        .add_property("parent", make_function(&gui_node_t::get_parent, return_value_policy<copy_const_reference>()))
        .add_property("dock_mode", &gui_node_t::get_dock_mode, &gui_node_t::set_dock_mode)
        .add_property("anchor_flags", &gui_node_t::get_anchor_flags, &gui_node_t::set_anchor_flags)
        .add_property("anchor_offset", make_function(&gui_node_t::get_anchor_offset, return_value_policy<copy_const_reference>()), &gui_node_t::set_anchor_offset)
        .add_property("padding", make_function(&gui_node_t::get_padding, return_value_policy<copy_const_reference>()), &gui_node_t::set_padding)
        .add_property("margin", make_function(&gui_node_t::get_margin, return_value_policy<copy_const_reference>()), &gui_node_t::set_margin)
        .def("set_size", &gui_node_t::set_size)
        .add_property("color", make_function(&gui_node_t::get_color, return_value_policy<copy_const_reference>()), &gui_node_t::set_color)
        .add_property("bounds", make_function(&gui_node_t::get_bounds, return_value_policy<copy_const_reference>()), &gui_node_t::set_bounds)
        .add_property("is_dirty", &gui_node_t::get_is_dirty)
        .add_property("is_hidden", &gui_node_t::get_is_hidden, &gui_node_t::set_is_hidden)
        .add_property("has_children", &gui_node_t::has_children)
        .add_property("has_parent", &gui_node_t::has_parent)
        .def("orphan", &gui_node_t::orphan)
        .def("dirty", &gui_node_t::dirty)
        .def("adopt", &gui_node_t::adopt);

    {
        scope gui_button_scope = class_<gui_button_t, bases<gui_node_t>, boost::shared_ptr<gui_button_t>, noncopyable>("GuiButton", init<>())
            .add_property("state", &gui_button_t::get_state);

        enum_<gui_button_t::state_t>("State")
            .value("IDLE", gui_button_t::state_t::idle)
#if defined(MANDALA_PC)
            .value("HOVER", gui_button_t::state_t::hover)
#endif
            .value("PRESSED", gui_button_t::state_t::pressed)
            .export_values();
    }

    class_<gui_canvas_t, bases<gui_node_t>, boost::shared_ptr<gui_canvas_t>, noncopyable>("GuiCanvas", init<>())
        .add_property("frame_buffer", make_function(&gui_canvas_t::get_frame_buffer, return_value_policy<copy_const_reference>()));

    {
        scope gui_label_scope = class_<gui_label_t, bases<gui_node_t>, boost::shared_ptr<gui_label_t>, noncopyable>("GuiLabel", init<>())
            .add_property("bitmap_font", make_function(&gui_label_t::get_bitmap_font, return_value_policy<copy_const_reference>()), &gui_label_t::set_bitmap_font)
            .add_property("string", make_function(&gui_label_t::get_string, return_value_policy<copy_const_reference>()), &gui_label_t::set_string)
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

        enum_<gui_label_t::vertical_alignment_e>("VerticalAlignment")
            .value("TOP", gui_label_t::vertical_alignment_e::top)
            .value("MIDDLE", gui_label_t::vertical_alignment_e::middle)
            .value("BOTTOM", gui_label_t::vertical_alignment_e::bottom)
            .export_values();
    }

    class_<rectangle_i16_t>("Rectangle", init<>())
        .def_readwrite("x", &rectangle_i16_t::x)
        .def_readwrite("y", &rectangle_i16_t::y)
        .def_readwrite("width", &rectangle_i16_t::width)
        .def_readwrite("height", &rectangle_i16_t::height);

    //STATES
    class_<state_t, boost::shared_ptr<state_t>, noncopyable>("StateBase", no_init);

    class_<state_wrapper_t, boost::shared_ptr<state_wrapper_t>, noncopyable>("State", init<>())
        .def("tick", &state_wrapper_t::tick)
        .def("on_input_event", &state_wrapper_t::on_input_event)
#if defined(MANDALA_PC)
        .def("on_window_event", &state_wrapper_t::on_window_event)
#endif
        .def("on_active", &state_wrapper_t::on_active)
        .def("on_passive", &state_wrapper_t::on_passive)
        .def("on_enter", &state_wrapper_t::on_enter)
        .def("on_exit", &state_wrapper_t::on_exit)
        .def("on_stop_tick", &state_wrapper_t::on_stop_tick)
        .def("on_start_tick", &state_wrapper_t::on_start_tick)
        .def("on_stop_render", &state_wrapper_t::on_stop_render)
        .def("on_start_render", &state_wrapper_t::on_start_render)
        .def("on_stop_input", &state_wrapper_t::on_stop_input)
        .def("on_start_input", &state_wrapper_t::on_start_input)
        ;

    class_<gui_layout_t, bases<gui_node_t>, boost::shared_ptr<gui_layout_t>, noncopyable>("GuiLayout", no_init);

    //RESOURCES
    class_<resource_t, boost::shared_ptr<resource_t>, noncopyable>("Resource", no_init);

    class_<bitmap_font_t, bases<resource_t>, boost::shared_ptr<bitmap_font_t>, noncopyable>("BitmapFont", no_init)
        .add_property("size", &bitmap_font_t::get_size)
        .add_property("is_smooth", &bitmap_font_t::get_is_smooth)
        .add_property("is_unicode", &bitmap_font_t::get_is_unicode)
        .add_property("is_bold", &bitmap_font_t::get_is_bold)
        .add_property("is_fixed_height", &bitmap_font_t::get_is_fixed_height)
        .add_property("char_set", &bitmap_font_t::get_char_set)
        .add_property("stretch_height", &bitmap_font_t::get_stretch_height)
        .add_property("antialiasing", &bitmap_font_t::get_antialiasing)
        .add_property("padding", make_function(&bitmap_font_t::get_padding, return_value_policy<copy_const_reference>()))
        .add_property("spacing_horizontal", &bitmap_font_t::get_spacing_horizontal)
        .add_property("spacing_vertical", &bitmap_font_t::get_spacing_vertical)
        .add_property("outline", &bitmap_font_t::get_outline)
        .add_property("line_height", &bitmap_font_t::get_line_height)
        .add_property("base", &bitmap_font_t::get_base)
        .add_property("width", &bitmap_font_t::get_width)
        .add_property("height", &bitmap_font_t::get_height)
        .add_property("page_count", &bitmap_font_t::get_page_count)
        .add_property("flags_1", &bitmap_font_t::get_flags_1)
        .add_property("alpha_channel", &bitmap_font_t::get_alpha_channel)
        .add_property("red_channel", &bitmap_font_t::get_red_channel)
        .add_property("green_channel", &bitmap_font_t::get_green_channel)
        .add_property("blue_channel", &bitmap_font_t::get_blue_channel);

    class_<texture_t, bases<resource_t>, boost::shared_ptr<texture_t>, noncopyable>("Texture", no_init)
        .add_property("size", make_function(&texture_t::get_size, return_value_policy<copy_const_reference>()))
        .add_property("id", &texture_t::get_id);

    {
        scope sprite_set_scope = class_<sprite_set_t, bases<resource_t>, boost::shared_ptr<sprite_set_t>, noncopyable>("SpriteSet", no_init)
            .add_property("texture", make_function(&sprite_set_t::get_texture, return_value_policy<copy_const_reference>()));
    }

    class_<sprite_t, noncopyable>("Sprite", init<const hash_t&, const hash_t&>())
        .add_property("region", make_function(&sprite_t::get_region, return_value_policy<copy_const_reference>()))
        .add_property("sprite_set", make_function(&sprite_t::get_sprite_set, return_value_policy<copy_const_reference>()));

    {
        scope gui_image_scope = class_<gui_image_t, bases<gui_node_t>, boost::shared_ptr<gui_image_t>, noncopyable>("GuiImage", init<>())
            .add_property("sprite", make_function(&gui_image_t::get_sprite, return_value_policy<copy_const_reference>()), &gui_image_t::set_sprite)
            .add_property("is_autosized_to_sprite", &gui_image_t::get_is_autosized_to_sprite, &gui_image_t::set_is_autosized_to_sprite)
            .add_property("triangle_mode", &gui_image_t::get_triangle_mode, &gui_image_t::set_triangle_mode);

        enum_<gui_image_t::triangle_mode_e>("TriangleMode")
            .value("BOTTOM_RIGHT", gui_image_t::triangle_mode_e::bottom_right)
            .value("TOP_LEFT", gui_image_t::triangle_mode_e::top_left)
            .value("TOP_RIGHT", gui_image_t::triangle_mode_e::top_right)
            .value("BOTTOM_LEFT", gui_image_t::triangle_mode_e::bottom_left)
            .value("BOTH", gui_image_t::triangle_mode_e::both);
    }

    enum_<gpu_frame_buffer_type_e>("GpuFrameBufferType")
        .value("COLOR", gpu_frame_buffer_type_e::color)
        .value("COLOR_DEPTH", gpu_frame_buffer_type_e::color_depth)
        .value("COLOR_DEPTH_STENCIL", gpu_frame_buffer_type_e::color_depth_stencil)
        .value("DEPTH", gpu_frame_buffer_type_e::depth)
        .value("DEPTH_STENCIL", gpu_frame_buffer_type_e::depth_stencil)
        .export_values();

    class_<frame_buffer_t, noncopyable>("FrameBuffer", no_init)
        .add_property("id", &frame_buffer_t::get_id)
        .add_property("color_texture", make_function(&frame_buffer_t::get_color_texture, return_value_policy<copy_const_reference>()))
        .add_property("depth_texture", make_function(&frame_buffer_t::get_depth_texture, return_value_policy<copy_const_reference>()))
        .add_property("depth_stencil_texture", make_function(&frame_buffer_t::get_depth_stencil_texture, return_value_policy<copy_const_reference>()))
        .add_property("size", make_function(&frame_buffer_t::get_size, return_value_policy<copy_const_reference>()), &frame_buffer_t::set_size)
        //.def_readonly("type", &frame_buffer_t::get_type)
        ;
}