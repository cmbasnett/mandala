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
#include "model.hpp"
#include "frame_buffer.hpp"
#include "gui_canvas.hpp"
#include "game.hpp"

//TODO: remove this
#include "armada\bsp_state.hpp"

using namespace boost;
using namespace boost::python;
using namespace mandala;

#define MANDALA_PYTHON_DECLARE_WRAPPER_CLASS(name) struct name##_wrapper_t : name##_t, wrapper<name##_t>

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

MANDALA_PYTHON_DECLARE_WRAPPER_CLASS(game)
{
    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, app_run_start)
    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, app_run_end)
    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, app_tick_start)
    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, app_tick_end)
    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, app_render_start)
    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, app_render_end)

    void on_input_event(input_event_t& input_event) override
    {
        auto override_ = get_override("on_input_event");

        if (override_)
        {
            override_(input_event);
        }
        else
        {
            game_t::on_input_event(input_event);
        }
    }
};

MANDALA_PYTHON_DECLARE_WRAPPER_CLASS(gui_state)
{
    void on_tick(float32_t dt) override
    {
        auto override_ = get_override("on_tick");

        if (override_)
        {
            override_(dt);
        }
        else
        {
            gui_state_t::on_tick(dt);
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
            gui_state_t::on_input_event(input_event);
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
MANDALA_DEFINE_RESOURCE_GET_FUNCTION(model)
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

#define MANDALA_PYTHON_DEFINE_AABB2(name, scalar_type)\
class_<mandala::details::aabb2_t<scalar_type>>(name, init<>())\
.def_readwrite("min", &mandala::details::aabb2_t<scalar_type>::min)\
.def_readwrite("max", &mandala::details::aabb2_t<scalar_type>::max)\
.add_property("width", &mandala::details::aabb2_t<scalar_type>::width)\
.add_property("height", &mandala::details::aabb2_t<scalar_type>::height)\
.add_property("size", make_function(&mandala::details::aabb2_t<scalar_type>::size, return_value_policy<return_by_value>()))\
.add_property("center", make_function(&mandala::details::aabb2_t<scalar_type>::center, return_value_policy<return_by_value>()))\
.def(self_ns::self + other<mandala::details::aabb2_t<scalar_type>::value_type>())\
.def(self_ns::self += other<mandala::details::aabb2_t<scalar_type>::value_type>())\
.def(self_ns::self - other<mandala::details::aabb2_t<scalar_type>::value_type>())\
.def(self_ns::self -= other<mandala::details::aabb2_t<scalar_type>::value_type>())\
.def(self_ns::str(self_ns::self));\

#define MANDALA_PYTHON_DEFINE_AABB3(name, scalar_type)\
class_<mandala::details::aabb3_t<scalar_type>>(name, init<>())\
.def_readwrite("min", &mandala::details::aabb3_t<scalar_type>::min)\
.def_readwrite("max", &mandala::details::aabb3_t<scalar_type>::max)\
.add_property("width", &mandala::details::aabb3_t<scalar_type>::width)\
.add_property("height", &mandala::details::aabb3_t<scalar_type>::height)\
.add_property("depth", &mandala::details::aabb3_t<scalar_type>::depth)\
.add_property("size", make_function(&mandala::details::aabb3_t<scalar_type>::size, return_value_policy<return_by_value>()))\
.add_property("center", make_function(&mandala::details::aabb3_t<scalar_type>::center, return_value_policy<return_by_value>()))\
.def(self_ns::self + other<mandala::details::aabb3_t<scalar_type>::value_type>())\
.def(self_ns::self += other<mandala::details::aabb3_t<scalar_type>::value_type>())\
.def(self_ns::self - other<mandala::details::aabb3_t<scalar_type>::value_type>())\
.def(self_ns::self -= other<mandala::details::aabb3_t<scalar_type>::value_type>());\

#define MANDALA_PYTHON_DEFINE_RECTANGLE(name, scalar_type)\
class_<mandala::details::rectangle_t<scalar_type>>(name, init<>())\
.def_readwrite("x", &mandala::details::rectangle_t<scalar_type>::x)\
.def_readwrite("y", &mandala::details::rectangle_t<scalar_type>::y)\
.def_readwrite("width", &mandala::details::rectangle_t<scalar_type>::width)\
.def_readwrite("height", &mandala::details::rectangle_t<scalar_type>::height);\

#define MANDALA_PYTHON_DEFINE_PADDING(name, scalar_type)\
class_<mandala::details::padding_t<scalar_type>>(name, init<scalar_type, scalar_type, scalar_type, scalar_type>())\
.def_readwrite("bottom", &mandala::details::padding_t<scalar_type>::bottom)\
.def_readwrite("left", &mandala::details::padding_t<scalar_type>::left)\
.def_readonly("top", &mandala::details::padding_t<scalar_type>::top)\
.def_readonly("right", &mandala::details::padding_t<scalar_type>::right)\
.add_property("vertical", &mandala::details::padding_t<scalar_type>::vertical)\
.add_property("horizontal", &mandala::details::padding_t<scalar_type>::horizontal)\
.add_property("size", &mandala::details::padding_t<scalar_type>::size)\
.def(self_ns::self + self_ns::self)\
.def(self_ns::self += self_ns::self)\
.def(self_ns::self - self_ns::self)\
.def(self_ns::self -= self_ns::self)\
.def(self_ns::str(self_ns::self));\

BOOST_PYTHON_MODULE(mandala)
{
    python_optional<sprite_t>();
    python_optional<size_t>();

    class_<game_wrapper_t, boost::shared_ptr<game_wrapper_t>, noncopyable>("Game", init<>())
        .add_property("name", make_function(&game_wrapper_t::get_name, return_value_policy<return_by_value>()));

    class_<hash_t>("Hash", init<std::string>());

    //MATH
    MANDALA_PYTHON_DEFINE_AABB2("AABB2I", int);
    MANDALA_PYTHON_DEFINE_AABB2("AABB2F", float);

    MANDALA_PYTHON_DEFINE_AABB3("AABB3I", int);
    MANDALA_PYTHON_DEFINE_AABB3("AABB3F", float);

    //move to GLM module?
    MANDALA_PYTHON_DEFINE_VEC2("Vec2I", int);
    MANDALA_PYTHON_DEFINE_VEC2("Vec2F", float);

    MANDALA_PYTHON_DEFINE_VEC3("Vec3I", int);
    MANDALA_PYTHON_DEFINE_VEC3("Vec3F", float);

    MANDALA_PYTHON_DEFINE_VEC4("Vec4I", int);
    MANDALA_PYTHON_DEFINE_VEC4("Vec4F", float);

    MANDALA_PYTHON_DEFINE_RECTANGLE("RectangleI", int);
    MANDALA_PYTHON_DEFINE_RECTANGLE("RectangleF", float);

    MANDALA_PYTHON_DEFINE_PADDING("PaddingI", int);
    MANDALA_PYTHON_DEFINE_PADDING("PaddingF", float);

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
        .add_property("window_size", &platform_t::get_window_size, &platform_t::set_window_size)
        .add_property("window_location", &platform_t::get_window_location, &platform_t::set_window_location)
#endif
        ;

    {
        scope input_event_scope = class_<input_event_t>("InputEvent", no_init)
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
                .def_readonly("key", &input_event_t::keyboard_t::key)
                .def_readonly("mod_flags", &input_event_t::keyboard_t::mod_flags)
                .def_readonly("character", &input_event_t::keyboard_t::character);

            enum_<input_event_t::keyboard_t::type_e>("Type")
                .value("NONE", input_event_t::keyboard_t::type_e::none)
                .value("KEY_RELEASE", input_event_t::keyboard_t::type_e::key_release)
                .value("KEY_PRESS", input_event_t::keyboard_t::type_e::key_press)
                .value("KEY_REPEAT", input_event_t::keyboard_t::type_e::key_repeat)
                .value("CHARACTER", input_event_t::keyboard_t::type_e::character)
                .export_values();

            enum_<input_event_t::keyboard_t::key_e>("Key")
                .value("NONE", input_event_t::keyboard_t::key_e::none)
                .value("SPACE", input_event_t::keyboard_t::key_e::space)
                .value("APOSTROPHE", input_event_t::keyboard_t::key_e::apostrophe)
                .value("COMMA", input_event_t::keyboard_t::key_e::comma)
                .value("MINUS", input_event_t::keyboard_t::key_e::minus)
                .value("PERIOD", input_event_t::keyboard_t::key_e::period)
                .value("SLASH", input_event_t::keyboard_t::key_e::slash)
                .value("NR_0", input_event_t::keyboard_t::key_e::nr_0)
                .value("NR_1", input_event_t::keyboard_t::key_e::nr_1)
                .value("NR_2", input_event_t::keyboard_t::key_e::nr_2)
                .value("NR_3", input_event_t::keyboard_t::key_e::nr_3)
                .value("NR_4", input_event_t::keyboard_t::key_e::nr_4)
                .value("NR_5", input_event_t::keyboard_t::key_e::nr_5)
                .value("NR_6", input_event_t::keyboard_t::key_e::nr_6)
                .value("NR_7", input_event_t::keyboard_t::key_e::nr_7)
                .value("NR_8", input_event_t::keyboard_t::key_e::nr_8)
                .value("NR_9", input_event_t::keyboard_t::key_e::nr_9)
                .value("SEMICOLON", input_event_t::keyboard_t::key_e::semicolon)
                .value("EQUAL", input_event_t::keyboard_t::key_e::equal)
                .value("A", input_event_t::keyboard_t::key_e::a)
                .value("B", input_event_t::keyboard_t::key_e::b)
                .value("C", input_event_t::keyboard_t::key_e::c)
                .value("D", input_event_t::keyboard_t::key_e::d)
                .value("E", input_event_t::keyboard_t::key_e::e)
                .value("F", input_event_t::keyboard_t::key_e::f)
                .value("G", input_event_t::keyboard_t::key_e::g)
                .value("H", input_event_t::keyboard_t::key_e::h)
                .value("I", input_event_t::keyboard_t::key_e::i)
                .value("J", input_event_t::keyboard_t::key_e::j)
                .value("K", input_event_t::keyboard_t::key_e::k)
                .value("L", input_event_t::keyboard_t::key_e::l)
                .value("M", input_event_t::keyboard_t::key_e::m)
                .value("N", input_event_t::keyboard_t::key_e::n)
                .value("O", input_event_t::keyboard_t::key_e::o)
                .value("P", input_event_t::keyboard_t::key_e::p)
                .value("Q", input_event_t::keyboard_t::key_e::q)
                .value("R", input_event_t::keyboard_t::key_e::r)
                .value("S", input_event_t::keyboard_t::key_e::s)
                .value("T", input_event_t::keyboard_t::key_e::t)
                .value("U", input_event_t::keyboard_t::key_e::u)
                .value("V", input_event_t::keyboard_t::key_e::v)
                .value("W", input_event_t::keyboard_t::key_e::w)
                .value("X", input_event_t::keyboard_t::key_e::x)
                .value("Y", input_event_t::keyboard_t::key_e::y)
                .value("Z", input_event_t::keyboard_t::key_e::z)
                .value("LEFT_BRACKET", input_event_t::keyboard_t::key_e::left_bracket)
                .value("BACKSLASH", input_event_t::keyboard_t::key_e::backslash)
                .value("RIGHT_BRACKET", input_event_t::keyboard_t::key_e::right_bracket)
                .value("GRAVE_ACCENT", input_event_t::keyboard_t::key_e::grave_accent)
                .value("WORLD_1", input_event_t::keyboard_t::key_e::world_1)
                .value("WORLD_2", input_event_t::keyboard_t::key_e::world_2)
                .value("ESCAPE", input_event_t::keyboard_t::key_e::escape)
                .value("ENTER", input_event_t::keyboard_t::key_e::enter)
                .value("TAB", input_event_t::keyboard_t::key_e::tab)
                .value("BACKSPACE", input_event_t::keyboard_t::key_e::backspace)
                .value("INSERT", input_event_t::keyboard_t::key_e::insert)
                .value("DEL", input_event_t::keyboard_t::key_e::del)
                .value("RIGHT", input_event_t::keyboard_t::key_e::right)
                .value("LEFT", input_event_t::keyboard_t::key_e::left)
                .value("DOWN", input_event_t::keyboard_t::key_e::down)
                .value("UP", input_event_t::keyboard_t::key_e::up)
                .value("PAGE_UP", input_event_t::keyboard_t::key_e::page_up)
                .value("PAGE_DOWN", input_event_t::keyboard_t::key_e::page_down)
                .value("HOME", input_event_t::keyboard_t::key_e::home)
                .value("END", input_event_t::keyboard_t::key_e::end)
                .value("CAPS_LOCK", input_event_t::keyboard_t::key_e::caps_lock)
                .value("SCROLL_LOCK", input_event_t::keyboard_t::key_e::scroll_lock)
                .value("NUM_LOCK", input_event_t::keyboard_t::key_e::num_lock)
                .value("PRINT_SCREEN", input_event_t::keyboard_t::key_e::print_screen)
                .value("PAUSE", input_event_t::keyboard_t::key_e::pause)
                .value("F1", input_event_t::keyboard_t::key_e::f1)
                .value("F2", input_event_t::keyboard_t::key_e::f2)
                .value("F3", input_event_t::keyboard_t::key_e::f3)
                .value("F4", input_event_t::keyboard_t::key_e::f4)
                .value("F5", input_event_t::keyboard_t::key_e::f5)
                .value("F6", input_event_t::keyboard_t::key_e::f6)
                .value("F7", input_event_t::keyboard_t::key_e::f7)
                .value("F8", input_event_t::keyboard_t::key_e::f8)
                .value("F9", input_event_t::keyboard_t::key_e::f9)
                .value("F10", input_event_t::keyboard_t::key_e::f10)
                .value("F11", input_event_t::keyboard_t::key_e::f11)
                .value("F12", input_event_t::keyboard_t::key_e::f12)
                .value("F13", input_event_t::keyboard_t::key_e::f13)
                .value("F14", input_event_t::keyboard_t::key_e::f14)
                .value("F15", input_event_t::keyboard_t::key_e::f15)
                .value("F16", input_event_t::keyboard_t::key_e::f16)
                .value("F17", input_event_t::keyboard_t::key_e::f17)
                .value("F18", input_event_t::keyboard_t::key_e::f18)
                .value("F19", input_event_t::keyboard_t::key_e::f19)
                .value("F20", input_event_t::keyboard_t::key_e::f20)
                .value("F21", input_event_t::keyboard_t::key_e::f21)
                .value("F22", input_event_t::keyboard_t::key_e::f22)
                .value("F23", input_event_t::keyboard_t::key_e::f23)
                .value("F24", input_event_t::keyboard_t::key_e::f24)
                .value("F25", input_event_t::keyboard_t::key_e::f25)
                .value("KP_0", input_event_t::keyboard_t::key_e::kp_0)
                .value("KP_1", input_event_t::keyboard_t::key_e::kp_1)
                .value("KP_2", input_event_t::keyboard_t::key_e::kp_2)
                .value("KP_3", input_event_t::keyboard_t::key_e::kp_3)
                .value("KP_4", input_event_t::keyboard_t::key_e::kp_4)
                .value("KP_5", input_event_t::keyboard_t::key_e::kp_5)
                .value("KP_6", input_event_t::keyboard_t::key_e::kp_6)
                .value("KP_7", input_event_t::keyboard_t::key_e::kp_7)
                .value("KP_8", input_event_t::keyboard_t::key_e::kp_8)
                .value("KP_9", input_event_t::keyboard_t::key_e::kp_9)
                .value("KP_DECIMAL", input_event_t::keyboard_t::key_e::kp_decimal)
                .value("KP_DIVIDE", input_event_t::keyboard_t::key_e::kp_divide)
                .value("KP_MULTIPLY", input_event_t::keyboard_t::key_e::kp_multiply)
                .value("KP_SUBTRACT", input_event_t::keyboard_t::key_e::kp_subtract)
                .value("KP_ADD", input_event_t::keyboard_t::key_e::kp_add)
                .value("KP_ENTER", input_event_t::keyboard_t::key_e::kp_enter)
                .value("KP_EQUAL", input_event_t::keyboard_t::key_e::kp_equal)
                .value("LEFT_SHIFT", input_event_t::keyboard_t::key_e::left_shift)
                .value("LEFT_CONTROL", input_event_t::keyboard_t::key_e::left_control)
                .value("LEFT_ALT", input_event_t::keyboard_t::key_e::left_alt)
                .value("LEFT_SUPER", input_event_t::keyboard_t::key_e::left_super)
                .value("RIGHT_SHIFT", input_event_t::keyboard_t::key_e::right_shift)
                .value("RIGHT_CONTROL", input_event_t::keyboard_t::key_e::right_control)
                .value("RIGHT_ALT", input_event_t::keyboard_t::key_e::right_alt)
                .value("RIGHT_SUPER", input_event_t::keyboard_t::key_e::right_super)
                .value("MENU", input_event_t::keyboard_t::key_e::menu)
                .export_values();
        }
#endif
    }

    scope().attr("platform") = boost::ref(platform);

    //APP
    {
        scope app_scope = class_<app_t, noncopyable>("App", no_init)
            .def("exit", &app_t::exit)
            .def("reset", &app_t::reset)
            .def("run", &app_t::run)
            .add_property("performance", make_function(&app_t::get_performance, return_value_policy<copy_const_reference>()));

        class_<app_t::performance_t>("Performance", no_init)
            .def_readonly("fps", &app_t::performance_t::fps);
    }

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
        .def("get_model", &resources_get_model)
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
            .add_property("is_read_only", &gui_label_t::get_is_read_only, &gui_label_t::set_is_read_only)
            .add_property("max_length", &gui_label_t::get_max_length, &gui_label_t::set_max_length)
            .add_property("is_autosized_to_text", &gui_label_t::get_is_autosized_to_text, &gui_label_t::set_is_autosized_to_text);

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

    class_<gui_layout_t, bases<gui_node_t>, boost::shared_ptr<gui_layout_t>, noncopyable>("GuiLayout", no_init);

    //STATES
    class_<state_t, boost::shared_ptr<state_t>, noncopyable>("StateBase", no_init);

    class_<gui_state_t, bases<state_t>, boost::shared_ptr<gui_state_t>, noncopyable>("GuiStateBase", no_init);

    class_<gui_state_wrapper_t, boost::shared_ptr<gui_state_wrapper_t>, noncopyable>("GuiState", init<>())
        .def("tick", &gui_state_wrapper_t::tick)
        .def("on_input_event", &gui_state_wrapper_t::on_input_event)
#if defined(MANDALA_PC)
        .def("on_window_event", &gui_state_wrapper_t::on_window_event)
#endif
        .def("on_active", &gui_state_wrapper_t::on_active)
        .def("on_passive", &gui_state_wrapper_t::on_passive)
        .def("on_enter", &gui_state_wrapper_t::on_enter)
        .def("on_exit", &gui_state_wrapper_t::on_exit)
        .def("on_stop_tick", &gui_state_wrapper_t::on_stop_tick)
        .def("on_start_tick", &gui_state_wrapper_t::on_start_tick)
        .def("on_stop_render", &gui_state_wrapper_t::on_stop_render)
        .def("on_start_render", &gui_state_wrapper_t::on_start_render)
        .def("on_stop_input", &gui_state_wrapper_t::on_stop_input)
        .def("on_start_input", &gui_state_wrapper_t::on_start_input)
        .add_property("layout", make_function(&gui_state_wrapper_t::get_layout, return_value_policy<copy_const_reference>()))
        ;

    class_<armada::bsp_state_t, bases<state_t>, boost::shared_ptr<armada::bsp_state_t>, noncopyable>("BspState", init<>());

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

    class_<model_t, bases<resource_t>, boost::shared_ptr<model_t>, noncopyable>("Model", no_init);

    //GPU
    class_<gpu_t, noncopyable>("Gpu", no_init)
        .add_property("vendor", make_function(&gpu_t::get_vendor, return_value_policy<copy_const_reference>()))
        .add_property("renderer", make_function(&gpu_t::get_renderer, return_value_policy<copy_const_reference>()))
        .add_property("shading_language_version", make_function(&gpu_t::get_shading_language_version, return_value_policy<copy_const_reference>()))
        .add_property("version", make_function(&gpu_t::get_version, return_value_policy<copy_const_reference>()))
        .add_property("extensions", make_function(&gpu_t::get_extensions, return_value_policy<copy_const_reference>()))
        .add_property("clear_color", make_function(&gpu_t::get_clear_color, return_value_policy<return_by_value>()), &gpu_t::set_clear_color);

    scope().attr("gpu") = boost::ref(gpu);

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
        .add_property("type", &frame_buffer_t::get_type)
        ;
}