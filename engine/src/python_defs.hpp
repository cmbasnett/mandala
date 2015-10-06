#pragma once

//boost
#include <boost/preprocessor.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

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
#include "state.hpp"
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
#include "python.hpp"
#include "closure_traits.hpp"
#include "cache_mgr.hpp"
#include "interpolation.hpp"
#include "scene.hpp"
#include "actor.hpp"
#include "pose.hpp"
#include "camera.hpp"
#include "quake_camera.hpp"
#include "arcball_camera.hpp"
#include "model_instance.hpp"
#include "gui_layout.hpp"

using namespace boost;
using namespace boost::python;
using namespace mandala;

//generate "type _#"
#define PARAMS(z,n,data) BOOST_PP_TUPLE_ELEM(n,data) _##n

//The first variant: with parameters
//parameters: PARAMS(z,0,(char,short,int)), PARAMS(z,1,(char,short,int)), PARAMS(z,2,(char,short,int))
//call: _0, _1, _2

#define DEFINE_FUNCTION_WRAPPER_WITH_PARAMS(return_type, name, ...)\
return_type name##_wrapper(BOOST_PP_ENUM(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), PARAMS, (__VA_ARGS__)))\
{\
    return name(BOOST_PP_ENUM_PARAMS(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), _));\
}

//The second variant: no parameters
#define DEFINE_FUNCTION_WRAPPER_WITHOUT_PARAMS(return_type, name)\
return_type name##_wrapper()\
{\
    return name();\
}

//choose variant based on whether more than two arguments are passed
#define DEFINE_FUNCTION_WRAPPER(...)\
    BOOST_PP_IF(\
        BOOST_PP_GREATER(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), 2), \
        DEFINE_FUNCTION_WRAPPER_WITH_PARAMS,\
        DEFINE_FUNCTION_WRAPPER_WITHOUT_PARAMS\
    )(__VA_ARGS__)

//Clang output:
//void foo_wrapper( char _0 , short _1 , int _2){ return foo( _0 , _1 , _2);}
//int bar_wrapper(){ return bar();}

#define MANDALA_PYTHON_DECLARE_WRAPPER_CLASS(name) struct name##_wrapper_t : name##_t, wrapper<name##_t>

#define MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(return_type, name, ...)\
return_type name(__VA_ARGS__) override\
{\
    auto override_ = get_override(BOOST_PP_STRINGIZE(name));\
    \
    if (override_)\
    {\
        override_();\
    }\
    else\
    {\
        _wrapper_wrapped_type_::name();\
    }\
}\
\
return_type name##_base(__VA_ARGS__)\
{\
    return _wrapper_wrapped_type_::name();\
}\

#define MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION_VIRTUAL(return_type, name, ...)\
return_type name(__VA_ARGS__) override\
{\
    auto override_ = get_override(BOOST_PP_STRINGIZE(name));\
    \
    if (override_)\
    {\
        override_();\
    }\
}\

MANDALA_PYTHON_DECLARE_WRAPPER_CLASS(game)
{
    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION_VIRTUAL(void, on_run_start)
    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION_VIRTUAL(void, on_run_end)

    void on_tick_start(float32_t dt) override
    {
        auto override_ = get_override("on_tick_start");

        if (override_)
        {
            override_(dt);
        }
    }

    void on_tick_end(float32_t dt) override
    {
        auto override_ = get_override("on_tick_end");

        if (override_)
        {
            override_(dt);
        }
    }

    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION_VIRTUAL(void, on_render_start)
    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION_VIRTUAL(void, on_render_end)

    bool on_input_event(input_event_t& input_event) override
    {
        auto override_ = get_override("on_input_event");

        if (override_)
        {
            return override_(input_event);
        }

        return false;
    }

#if defined(MANDALA_PC)
    void on_window_event(window_event_t& window_event) override
    {
        auto override_ = get_override("on_window_event");

        if (override_)
        {
            override_(window_event);
        }
    }
#endif
};

MANDALA_PYTHON_DECLARE_WRAPPER_CLASS(state)
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
            _wrapper_wrapped_type_::on_tick(dt);
        }
    }

    void on_tick_base(float32_t dt)
    {
        _wrapper_wrapped_type_::on_tick(dt);
    }

    bool on_input_event(input_event_t& input_event) override
    {
        auto override_ = get_override("on_input_event");

        if (override_)
        {
            return override_(input_event);
        }
        else
        {
            return state_t::on_input_event(input_event);
        }
    }

    bool on_input_event_base(input_event_t& input_event)
    {
        return _wrapper_wrapped_type_::on_input_event(input_event);
    }

    MANDALA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, render)
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
    .def(self_ns::str(self_ns::self))\
    .def(self_ns::repr(self_ns::self));\

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

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(state_mgr_push_overloads, state_mgr_t::push, 2, 3)

BOOST_PYTHON_MODULE(mandala)
{
    python_optional<sprite_t>();
    python_optional<size_t>();

    class_<python_t, noncopyable>("Python", no_init)
        .def("execute", &python_t::exec)
        .def("exec_file", &python_t::exec_file);

    scope().attr("py") = boost::ref(py);

    class_<game_wrapper_t, boost::shared_ptr<game_wrapper_t>, noncopyable>("Game", init<>())
        .def("on_run_start", pure_virtual(&game_t::on_run_start))
        .def("on_run_end", pure_virtual(&game_t::on_run_end))
        .def("on_tick_start", pure_virtual(&game_t::on_tick_start))
        .def("on_tick_end", pure_virtual(&game_t::on_tick_end))
        .def("on_render_start", pure_virtual(&game_t::on_render_start))
        .def("on_render_end", pure_virtual(&game_t::on_render_end));

    class_<hash_t>("Hash", init<std::string>());

    //MATH
    MANDALA_PYTHON_DEFINE_AABB2("AABB2I", int);
    MANDALA_PYTHON_DEFINE_AABB2("AABB2F", float);

    MANDALA_PYTHON_DEFINE_AABB3("AABB3I", int);
    MANDALA_PYTHON_DEFINE_AABB3("AABB3F", float);

    //move to GLM module?
    MANDALA_PYTHON_DEFINE_VEC2("Vec2I", int);
    MANDALA_PYTHON_DEFINE_VEC2("Vec2F", float);
    MANDALA_PYTHON_DEFINE_VEC2("Vec2D", double);

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
    {
        scope platform_scope = 
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
            //.add_property("display", &platform_t::get_display, &platform_t::set_window_location)
            .add_property("fullscreen", &platform_t::is_fullscreen, &platform_t::set_is_fullscreen)
            .def_readwrite("is_cursor_centered", &platform_t::is_cursor_centered)
            .add_property("is_cursor_hidden", &platform_t::is_cursor_hidden, &platform_t::set_cursor_hidden)
#endif
            .add_property("clipboard_string", &platform_t::get_clipboard_string, &platform_t::set_clipboard_string)
            ;

        class_<platform_t::display_t, noncopyable>("PlatformDisplay", no_init)
            .def_readonly("name", &platform_t::display_t::name)
            .def_readonly("position", &platform_t::display_t::position)
            .def_readonly("ppi", &platform_t::display_t::position)
            //.add_property("ppi", make_getter(&platform_t::display_t::ppi, return_value_policy<copy_const_reference>()))
            ;
    }

    {
        scope input_event_scope = class_<input_event_t>("InputEvent", no_init)
            .add_property("id", &input_event_t::id)
            .add_property("device_type", &input_event_t::device_type)
            .add_property("touch", &input_event_t::touch)
            .add_property("keyboard", &input_event_t::keyboard)
            //#if defined(MANDALA_PC)
            //.def_readonly("gamepad", &input_event_t::gamepad)
            //#endif
            ;

        scope().attr("MOD_FLAG_SHIFT") = input_event_t::mod_flags_type(input_event_t::MOD_FLAG_SHIFT);
        scope().attr("MOD_FLAG_CTRL") = input_event_t::mod_flags_type(input_event_t::MOD_FLAG_CTRL);
        scope().attr("MOD_FLAG_ALT") = input_event_t::mod_flags_type(input_event_t::MOD_FLAG_ALT);
        scope().attr("MOD_FLAG_SUPER") = input_event_t::mod_flags_type(input_event_t::MOD_FLAG_SUPER);

        enum_<input_event_t::device_type_e>("DeviceType")
            .value("NONE", input_event_t::device_type_e::NONE)
            .value("TOUCH", input_event_t::device_type_e::TOUCH)
            .value("KEYBOARD", input_event_t::device_type_e::KEYBOARD)
#if defined(MANDALA_PC)
            .value("GAMEPAD", input_event_t::device_type_e::GAMEPAD)
#endif
            .value("COUNT", input_event_t::device_type_e::COUNT)
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
                .value("NONE", input_event_t::touch_t::type_e::NONE)
                .value("PRESS", input_event_t::touch_t::type_e::PRESS)
                .value("RELEASE", input_event_t::touch_t::type_e::RELEASE)
                .value("SCROLL", input_event_t::touch_t::type_e::SCROLL)
                .value("MOVE", input_event_t::touch_t::type_e::MOVE)
                .export_values();

#if defined(MANDALA_PC)
            enum_<input_event_t::touch_t::button_e>("Button")
                .value("NONE", input_event_t::touch_t::button_e::NONE)
                .value("LEFT", input_event_t::touch_t::button_e::LEFT)
                .value("RIGHT", input_event_t::touch_t::button_e::RIGHT)
                .value("MIDDLE", input_event_t::touch_t::button_e::MIDDLE)
                .value("FOUR", input_event_t::touch_t::button_e::FOUR)
                .value("FIVE", input_event_t::touch_t::button_e::FIVE)
                .value("SIX", input_event_t::touch_t::button_e::SIX)
                .value("SEVEN", input_event_t::touch_t::button_e::SEVEN)
                .value("EIGHT", input_event_t::touch_t::button_e::EIGHT)
                .value("COUNT", input_event_t::touch_t::button_e::COUNT)
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
                .value("NONE", input_event_t::keyboard_t::type_e::NONE)
                .value("KEY_RELEASE", input_event_t::keyboard_t::type_e::KEY_RELEASE)
                .value("KEY_PRESS", input_event_t::keyboard_t::type_e::KEY_PRESS)
                .value("KEY_REPEAT", input_event_t::keyboard_t::type_e::KEY_REPEAT)
                .value("CHARACTER", input_event_t::keyboard_t::type_e::CHARACTER)
                .export_values();

            enum_<input_event_t::keyboard_t::key_e>("Key")
                .value("NONE", input_event_t::keyboard_t::key_e::NONE)
                .value("SPACE", input_event_t::keyboard_t::key_e::SPACE)
                .value("APOSTROPHE", input_event_t::keyboard_t::key_e::APOSTROPHE)
                .value("COMMA", input_event_t::keyboard_t::key_e::COMMA)
                .value("MINUS", input_event_t::keyboard_t::key_e::MINUS)
                .value("PERIOD", input_event_t::keyboard_t::key_e::PERIOD)
                .value("SLASH", input_event_t::keyboard_t::key_e::SLASH)
                .value("NR_0", input_event_t::keyboard_t::key_e::NR_0)
                .value("NR_1", input_event_t::keyboard_t::key_e::NR_1)
                .value("NR_2", input_event_t::keyboard_t::key_e::NR_2)
                .value("NR_3", input_event_t::keyboard_t::key_e::NR_3)
                .value("NR_4", input_event_t::keyboard_t::key_e::NR_4)
                .value("NR_5", input_event_t::keyboard_t::key_e::NR_5)
                .value("NR_6", input_event_t::keyboard_t::key_e::NR_6)
                .value("NR_7", input_event_t::keyboard_t::key_e::NR_7)
                .value("NR_8", input_event_t::keyboard_t::key_e::NR_8)
                .value("NR_9", input_event_t::keyboard_t::key_e::NR_9)
                .value("SEMICOLON", input_event_t::keyboard_t::key_e::SEMICOLON)
                .value("EQUAL", input_event_t::keyboard_t::key_e::EQUAL)
                .value("A", input_event_t::keyboard_t::key_e::A)
                .value("B", input_event_t::keyboard_t::key_e::B)
                .value("C", input_event_t::keyboard_t::key_e::C)
                .value("D", input_event_t::keyboard_t::key_e::D)
                .value("E", input_event_t::keyboard_t::key_e::E)
                .value("F", input_event_t::keyboard_t::key_e::F)
                .value("G", input_event_t::keyboard_t::key_e::G)
                .value("H", input_event_t::keyboard_t::key_e::H)
                .value("I", input_event_t::keyboard_t::key_e::I)
                .value("J", input_event_t::keyboard_t::key_e::J)
                .value("K", input_event_t::keyboard_t::key_e::K)
                .value("L", input_event_t::keyboard_t::key_e::L)
                .value("M", input_event_t::keyboard_t::key_e::M)
                .value("N", input_event_t::keyboard_t::key_e::N)
                .value("O", input_event_t::keyboard_t::key_e::O)
                .value("P", input_event_t::keyboard_t::key_e::P)
                .value("Q", input_event_t::keyboard_t::key_e::Q)
                .value("R", input_event_t::keyboard_t::key_e::R)
                .value("S", input_event_t::keyboard_t::key_e::S)
                .value("T", input_event_t::keyboard_t::key_e::T)
                .value("U", input_event_t::keyboard_t::key_e::U)
                .value("V", input_event_t::keyboard_t::key_e::V)
                .value("W", input_event_t::keyboard_t::key_e::W)
                .value("X", input_event_t::keyboard_t::key_e::X)
                .value("Y", input_event_t::keyboard_t::key_e::Y)
                .value("Z", input_event_t::keyboard_t::key_e::Z)
                .value("LEFT_BRACKET", input_event_t::keyboard_t::key_e::LEFT_BRACKET)
                .value("BACKSLASH", input_event_t::keyboard_t::key_e::BACKSLASH)
                .value("RIGHT_BRACKET", input_event_t::keyboard_t::key_e::RIGHT_BRACKET)
                .value("GRAVE_ACCENT", input_event_t::keyboard_t::key_e::GRAVE_ACCENT)
                .value("WORLD_1", input_event_t::keyboard_t::key_e::WORLD_1)
                .value("WORLD_2", input_event_t::keyboard_t::key_e::WORLD_2)
                .value("ESCAPE", input_event_t::keyboard_t::key_e::ESCAPE)
                .value("ENTER", input_event_t::keyboard_t::key_e::ENTER)
                .value("TAB", input_event_t::keyboard_t::key_e::TAB)
                .value("BACKSPACE", input_event_t::keyboard_t::key_e::BACKSPACE)
                .value("INSERT", input_event_t::keyboard_t::key_e::INSERT)
                .value("DEL", input_event_t::keyboard_t::key_e::DEL)
                .value("RIGHT", input_event_t::keyboard_t::key_e::RIGHT)
                .value("LEFT", input_event_t::keyboard_t::key_e::LEFT)
                .value("DOWN", input_event_t::keyboard_t::key_e::DOWN)
                .value("UP", input_event_t::keyboard_t::key_e::UP)
                .value("PAGE_UP", input_event_t::keyboard_t::key_e::PAGE_UP)
                .value("PAGE_DOWN", input_event_t::keyboard_t::key_e::PAGE_DOWN)
                .value("HOME", input_event_t::keyboard_t::key_e::HOME)
                .value("END", input_event_t::keyboard_t::key_e::END)
                .value("CAPS_LOCK", input_event_t::keyboard_t::key_e::CAPS_LOCK)
                .value("SCROLL_LOCK", input_event_t::keyboard_t::key_e::SCROLL_LOCK)
                .value("NUM_LOCK", input_event_t::keyboard_t::key_e::NUM_LOCK)
                .value("PRINT_SCREEN", input_event_t::keyboard_t::key_e::PRINT_SCREEN)
                .value("PAUSE", input_event_t::keyboard_t::key_e::PAUSE)
                .value("F1", input_event_t::keyboard_t::key_e::F1)
                .value("F2", input_event_t::keyboard_t::key_e::F2)
                .value("F3", input_event_t::keyboard_t::key_e::F3)
                .value("F4", input_event_t::keyboard_t::key_e::F4)
                .value("F5", input_event_t::keyboard_t::key_e::F5)
                .value("F6", input_event_t::keyboard_t::key_e::F6)
                .value("F7", input_event_t::keyboard_t::key_e::F7)
                .value("F8", input_event_t::keyboard_t::key_e::F8)
                .value("F9", input_event_t::keyboard_t::key_e::F9)
                .value("F10", input_event_t::keyboard_t::key_e::F10)
                .value("F11", input_event_t::keyboard_t::key_e::F11)
                .value("F12", input_event_t::keyboard_t::key_e::F12)
                .value("F13", input_event_t::keyboard_t::key_e::F13)
                .value("F14", input_event_t::keyboard_t::key_e::F14)
                .value("F15", input_event_t::keyboard_t::key_e::F15)
                .value("F16", input_event_t::keyboard_t::key_e::F16)
                .value("F17", input_event_t::keyboard_t::key_e::F17)
                .value("F18", input_event_t::keyboard_t::key_e::F18)
                .value("F19", input_event_t::keyboard_t::key_e::F19)
                .value("F20", input_event_t::keyboard_t::key_e::F20)
                .value("F21", input_event_t::keyboard_t::key_e::F21)
                .value("F22", input_event_t::keyboard_t::key_e::F22)
                .value("F23", input_event_t::keyboard_t::key_e::F23)
                .value("F24", input_event_t::keyboard_t::key_e::F24)
                .value("F25", input_event_t::keyboard_t::key_e::F25)
                .value("KP_0", input_event_t::keyboard_t::key_e::KP_0)
                .value("KP_1", input_event_t::keyboard_t::key_e::KP_1)
                .value("KP_2", input_event_t::keyboard_t::key_e::KP_2)
                .value("KP_3", input_event_t::keyboard_t::key_e::KP_3)
                .value("KP_4", input_event_t::keyboard_t::key_e::KP_4)
                .value("KP_5", input_event_t::keyboard_t::key_e::KP_5)
                .value("KP_6", input_event_t::keyboard_t::key_e::KP_6)
                .value("KP_7", input_event_t::keyboard_t::key_e::KP_7)
                .value("KP_8", input_event_t::keyboard_t::key_e::KP_8)
                .value("KP_9", input_event_t::keyboard_t::key_e::KP_9)
                .value("KP_DECIMAL", input_event_t::keyboard_t::key_e::KP_DECIMAL)
                .value("KP_DIVIDE", input_event_t::keyboard_t::key_e::KP_DIVIDE)
                .value("KP_MULTIPLY", input_event_t::keyboard_t::key_e::KP_MULTIPLY)
                .value("KP_SUBTRACT", input_event_t::keyboard_t::key_e::KP_SUBTRACT)
                .value("KP_ADD", input_event_t::keyboard_t::key_e::KP_ADD)
                .value("KP_ENTER", input_event_t::keyboard_t::key_e::KP_ENTER)
                .value("KP_EQUAL", input_event_t::keyboard_t::key_e::KP_EQUAL)
                .value("LEFT_SHIFT", input_event_t::keyboard_t::key_e::LEFT_SHIFT)
                .value("LEFT_CONTROL", input_event_t::keyboard_t::key_e::LEFT_CONTROL)
                .value("LEFT_ALT", input_event_t::keyboard_t::key_e::LEFT_ALT)
                .value("LEFT_SUPER", input_event_t::keyboard_t::key_e::LEFT_SUPER)
                .value("RIGHT_SHIFT", input_event_t::keyboard_t::key_e::RIGHT_SHIFT)
                .value("RIGHT_CONTROL", input_event_t::keyboard_t::key_e::RIGHT_CONTROL)
                .value("RIGHT_ALT", input_event_t::keyboard_t::key_e::RIGHT_ALT)
                .value("RIGHT_SUPER", input_event_t::keyboard_t::key_e::RIGHT_SUPER)
                .value("MENU", input_event_t::keyboard_t::key_e::MENU)
                .export_values();
        }
#endif
    }

#if defined(MANDALA_PC)
    {
        scope window_event_scope = class_<window_event_t>("WindowEvent", no_init)
            .add_property("type", &window_event_t::type)
            .add_property("rectangle", &window_event_t::rectangle);

        enum_<window_event_t::type_e>("Type")
            .value("MOVE", window_event_t::type_e::MOVE)
            .value("RESIZE", window_event_t::type_e::RESIZE)
            .export_values();
    }
#endif

    scope().attr("platform") = boost::ref(platform);

    //APP
    {
        scope app_scope = class_<app_t, noncopyable>("App", no_init)
            .def("exit", &app_t::exit)
            .def("reset", &app_t::reset)
            .def("run", &app_t::run)
            .add_property("performance", make_function(&app_t::get_performance, return_value_policy<copy_const_reference>()))
            .add_property("uptime", &app_t::get_uptime);

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
    scope().attr("STATE_FLAG_NONE") = state_flags_type(STATE_FLAG_NONE);
    scope().attr("STATE_FLAG_RENDER") = state_flags_type(STATE_FLAG_RENDER);
    scope().attr("STATE_FLAG_INPUT") = state_flags_type(STATE_FLAG_INPUT);
    scope().attr("STATE_FLAG_TICK") = state_flags_type(STATE_FLAG_TICK);
    scope().attr("STATE_FLAG_ALL") = state_flags_type(STATE_FLAG_ALL);

    class_<state_mgr_t, noncopyable>("StateMgr", no_init)
        .def("push", &state_mgr_t::push, state_mgr_push_overloads(boost::python::args("state", "link_flags", "weight")))
        .def("pop", &state_mgr_t::pop)
        .def("change_link_flags", &state_mgr_t::change_link_flags)
        .def("change_weight", &state_mgr_t::change_weight)
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

    //CACHE
    class_<cache_mgr_t, noncopyable>("CacheMgr", no_init)
        .def("purge", &cache_mgr_t::purge);

    scope().attr("cache") = boost::ref(cache);

    //GUI
    scope().attr("GUI_ANCHOR_FLAG_NONE") = gui_anchor_flags_type(GUI_ANCHOR_FLAG_NONE);
    scope().attr("GUI_ANCHOR_FLAG_BOTTOM") = gui_anchor_flags_type(GUI_ANCHOR_FLAG_BOTTOM);
    scope().attr("GUI_ANCHOR_FLAG_LEFT") = gui_anchor_flags_type(GUI_ANCHOR_FLAG_LEFT);
    scope().attr("GUI_ANCHOR_FLAG_RIGHT") = gui_anchor_flags_type(GUI_ANCHOR_FLAG_RIGHT);
    scope().attr("GUI_ANCHOR_FLAG_TOP") = gui_anchor_flags_type(GUI_ANCHOR_FLAG_TOP);
    scope().attr("GUI_ANCHOR_FLAG_VERTICAL") = gui_anchor_flags_type(GUI_ANCHOR_FLAG_VERTICAL);
    scope().attr("GUI_ANCHOR_FLAG_HORIZONTAL") = gui_anchor_flags_type(GUI_ANCHOR_FLAG_HORIZONTAL);
    scope().attr("GUI_ANCHOR_FLAG_ALL") = gui_anchor_flags_type(GUI_ANCHOR_FLAG_ALL);

    enum_<gui_size_mode_e>("GuiSizeMode")
        .value("ABSOLUTE", gui_size_mode_e::ABSOLUTE)
        .value("AXIS_SCALE", gui_size_mode_e::AXIS_SCALE)
        .value("INHERIT", gui_size_mode_e::INHERIT)
        .value("RELATIVE", gui_size_mode_e::RELATIVE)
        .export_values();

    enum_<gui_dock_mode_e>("GuiDockMode")
        .value("NONE", gui_dock_mode_e::NONE)
        .value("BOTTOM", gui_dock_mode_e::BOTTOM)
        .value("FILL", gui_dock_mode_e::FILL)
        .value("LEFT", gui_dock_mode_e::LEFT)
        .value("RIGHT", gui_dock_mode_e::RIGHT)
        .value("TOP", gui_dock_mode_e::TOP)
        .export_values();

    enum_<gui_visiblity_e>("GuiVisibility")
        .value("OMIT", gui_visiblity_e::OMIT)
        .value("HIDDEN", gui_visiblity_e::HIDDEN)
        .value("VISIBLE", gui_visiblity_e::VISIBLE)
        .export_values();

    class_<gui_size_modes_t>("GuiSizeModes", init<gui_size_mode_e, gui_size_mode_e>())
        .add_property("x", make_function(&gui_size_modes_t::get_x), &gui_size_modes_t::set_x)
        .add_property("y", make_function(&gui_size_modes_t::get_y), &gui_size_modes_t::set_y)
        ;

    class_<frame_buffer_t, boost::shared_ptr<frame_buffer_t>, noncopyable>("FrameBuffer", no_init);

    class_<gui_node_t, boost::shared_ptr<gui_node_t>, noncopyable>("GuiNode", init<>())
        .add_property("root", make_function(&gui_node_t::get_root, return_value_policy<copy_const_reference>()))
        .add_property("parent", make_function(&gui_node_t::get_parent, return_value_policy<copy_const_reference>()))
        .add_property("dock_mode", &gui_node_t::get_dock_mode, &gui_node_t::set_dock_mode)
        .add_property("anchor_flags", &gui_node_t::get_anchor_flags, &gui_node_t::set_anchor_flags)
        .add_property("anchor_offset", make_function(&gui_node_t::get_anchor_offset, return_value_policy<copy_const_reference>()), &gui_node_t::set_anchor_offset)
        .add_property("padding", make_function(&gui_node_t::get_padding, return_value_policy<copy_const_reference>()), &gui_node_t::set_padding)
        .add_property("margin", make_function(&gui_node_t::get_margin, return_value_policy<copy_const_reference>()), &gui_node_t::set_margin)
        .add_property("size", make_function(&gui_node_t::get_size, return_value_policy<copy_const_reference>()), &gui_node_t::set_size)
        .add_property("size_modes", make_function(&gui_node_t::get_size_modes, return_value_policy<copy_const_reference>()), &gui_node_t::set_size_modes)
        .add_property("color", make_function(&gui_node_t::get_color, return_value_policy<copy_const_reference>()), &gui_node_t::set_color)
        .add_property("bounds", make_function(&gui_node_t::get_bounds, return_value_policy<copy_const_reference>()), &gui_node_t::set_bounds)
        .add_property("should_clip", &gui_node_t::get_should_clip, &gui_node_t::set_should_clip)
        .add_property("is_dirty", &gui_node_t::get_is_dirty)
        .add_property("visibility", &gui_node_t::get_visibility, &gui_node_t::set_visibility)
        .add_property("has_children", &gui_node_t::has_children)
        .add_property("has_parent", &gui_node_t::has_parent)
        .def("orphan", &gui_node_t::orphan)
        .def("dirty", &gui_node_t::dirty)
        .def("adopt", &gui_node_t::adopt)
        ;

    {
        scope gui_button_scope = class_<gui_button_t, bases<gui_node_t>, boost::shared_ptr<gui_button_t>, noncopyable>("GuiButton", init<>())
            .add_property("state", &gui_button_t::get_state)
            .def("on_state_changed", WRAP_MEM(gui_button_t, on_state_changed))
            .def("set_on_state_changed", SET_MEM(gui_button_t, on_state_changed))
            ;

        enum_<gui_button_t::state_t>("State")
            .value("IDLE", gui_button_t::state_t::IDLE)
#if defined(MANDALA_PC)
            .value("HOVER", gui_button_t::state_t::HOVER)
#endif
            .value("PRESSED", gui_button_t::state_t::PRESSED)
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
            .add_property("is_autosized_to_text", &gui_label_t::get_is_autosized_to_text, &gui_label_t::set_is_autosized_to_text)
            .add_property("is_obscured", &gui_label_t::get_is_obscured, &gui_label_t::set_is_obscured)
            .def("escape_string", &gui_label_t::escape_string, return_value_policy<copy_non_const_reference>())
            .staticmethod("escape_string");


        enum_<gui_label_t::justification_e>("Justification")
            .value("LEFT", gui_label_t::justification_e::LEFT)
            .value("CENTER", gui_label_t::justification_e::CENTER)
            .value("RIGHT", gui_label_t::justification_e::RIGHT)
            .export_values();

        enum_<gui_label_t::vertical_alignment_e>("VerticalAlignment")
            .value("TOP", gui_label_t::vertical_alignment_e::TOP)
            .value("MIDDLE", gui_label_t::vertical_alignment_e::MIDDLE)
            .value("BOTTOM", gui_label_t::vertical_alignment_e::BOTTOM)
            .export_values();
    }
    
    {
        scope gui_image_scope = class_<gui_image_t, bases<gui_node_t>, boost::shared_ptr<gui_image_t>, noncopyable>("GuiImage", init<>())
            .add_property("sprite", make_function(&gui_image_t::get_sprite, return_value_policy<copy_const_reference>()), &gui_image_t::set_sprite)
            .add_property("is_autosized_to_sprite", &gui_image_t::get_is_autosized_to_sprite, &gui_image_t::set_is_autosized_to_sprite)
            .add_property("triangle_mode", &gui_image_t::get_triangle_mode, &gui_image_t::set_triangle_mode)
            .add_property("texcoord_scale", make_function(&gui_image_t::get_texcoord_scale, return_value_policy<copy_const_reference>()), &gui_image_t::set_texcoord_scale)
            .add_property("texcoord_origin", make_function(&gui_image_t::get_texcoord_origin, return_value_policy<copy_const_reference>()), &gui_image_t::set_texcoord_origin)
            .add_property("texcoord_rotation", &gui_image_t::get_texcoord_rotation, &gui_image_t::set_texcoord_rotation)
            .add_property("slice_padding", make_function(&gui_image_t::get_slice_padding, return_value_policy<copy_const_reference>()), &gui_image_t::set_slice_padding);

        enum_<gui_image_t::triangle_mode_e>("TriangleMode")
            .value("BOTTOM_RIGHT", gui_image_t::triangle_mode_e::BOTTOM_RIGHT)
            .value("TOP_LEFT", gui_image_t::triangle_mode_e::TOP_LEFT)
            .value("TOP_RIGHT", gui_image_t::triangle_mode_e::TOP_RIGHT)
            .value("BOTTOM_LEFT", gui_image_t::triangle_mode_e::BOTTOM_LEFT)
            .value("BOTH", gui_image_t::triangle_mode_e::BOTH)
            .value("SLICE", gui_image_t::triangle_mode_e::SLICE);
    }

    class_<gui_layout_t, bases<gui_node_t>, boost::shared_ptr<gui_layout_t>, noncopyable>("GuiLayout", no_init);

    //STATES
    class_<state_t, boost::shared_ptr<state_t>, noncopyable>("StateBase", no_init);

    class_<state_wrapper_t, boost::shared_ptr<state_wrapper_t>, noncopyable>("State", init<>())
        .def("tick", &state_wrapper_t::tick)
        .def("on_input_event", &state_wrapper_t::on_input_event)
        .def("on_input_event_base", &state_wrapper_t::on_input_event_base)
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
        .def("render_base", &state_wrapper_t::render_base)
        .add_property("layout", make_function(&state_wrapper_t::get_layout, return_value_policy<copy_const_reference>()));

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

    class_<std::map<const hash_t, boost::shared_ptr<sprite_set_t::region_t>>>("SpriteSetRegionMap")
        .def(map_indexing_suite<std::map<const hash_t, boost::shared_ptr<sprite_set_t::region_t>>>());

    class_<texture_t, bases<resource_t>, boost::shared_ptr<texture_t>, noncopyable>("Texture", no_init)
        .add_property("size", make_function(&texture_t::get_size, return_value_policy<copy_const_reference>()))
        .add_property("id", &texture_t::get_id);

    class_<sprite_set_t::region_t, boost::shared_ptr<sprite_set_t::region_t>, noncopyable>("SpriteSetRegion", no_init)
        .add_property("hash", &sprite_set_t::region_t::hash)
        .add_property("frame_rectangle", &sprite_set_t::region_t::frame_rectangle)
        .add_property("rectangle", &sprite_set_t::region_t::rectangle)
        .add_property("source_size", &sprite_set_t::region_t::source_size)
        .add_property("is_rotated", &sprite_set_t::region_t::is_rotated)
        .add_property("is_trimmed", &sprite_set_t::region_t::is_trimmed)
        .add_property("frame_uv", &sprite_set_t::region_t::frame_uv)
        .add_property("uv", &sprite_set_t::region_t::uv)
        ;

    {
        scope sprite_set_scope = class_<sprite_set_t, bases<resource_t>, boost::shared_ptr<sprite_set_t>, noncopyable>("SpriteSet", no_init)
            .add_property("texture", make_function(&sprite_set_t::get_texture, return_value_policy<copy_const_reference>()))
            .add_property("regions", make_function(&sprite_set_t::get_regions, return_value_policy<copy_const_reference>()))
            ;
    }

    class_<sprite_t, noncopyable>("Sprite", init<const hash_t&, const hash_t&>())
        .add_property("region", make_function(&sprite_t::get_region, return_value_policy<copy_const_reference>()))
        .add_property("sprite_set", make_function(&sprite_t::get_sprite_set, return_value_policy<copy_const_reference>()));

    class_<std::vector<const std::string>>("StringVec")
        .def(vector_indexing_suite<std::vector<const std::string>>());

    class_<model_t, bases<resource_t>, boost::shared_ptr<model_t>, noncopyable>("Model", no_init)
        .add_property("bones", make_function(&model_t::get_bone_names, return_value_policy<copy_const_reference>()));

    class_<bsp_t, bases<resource_t>, boost::shared_ptr<bsp_t>, noncopyable>("Bsp", no_init);

    //GPU
    class_<gpu_t, noncopyable>("Gpu", no_init)
        .add_property("vendor", make_function(&gpu_t::get_vendor, return_value_policy<copy_const_reference>()))
        .add_property("renderer", make_function(&gpu_t::get_renderer, return_value_policy<copy_const_reference>()))
        .add_property("shading_language_version", make_function(&gpu_t::get_shading_language_version, return_value_policy<copy_const_reference>()))
        .add_property("version", make_function(&gpu_t::get_version, return_value_policy<copy_const_reference>()))
        .add_property("extensions", make_function(&gpu_t::get_extensions, return_value_policy<copy_const_reference>()))
        .add_property("clear_color", make_function(&gpu_t::get_clear_color, return_value_policy<return_by_value>()), &gpu_t::set_clear_color);

    scope().attr("gpu") = boost::ref(gpu);

    enum_<gpu_frame_buffer_type_e>("FrameBufferType")
        .value("COLOR", gpu_frame_buffer_type_e::COLOR)
        .value("COLOR_DEPTH", gpu_frame_buffer_type_e::COLOR_DEPTH)
        .value("COLOR_DEPTH_STENCIL", gpu_frame_buffer_type_e::COLOR_DEPTH_STENCIL)
        .value("DEPTH", gpu_frame_buffer_type_e::DEPTH)
        .value("DEPTH_STENCIL", gpu_frame_buffer_type_e::DEPTH_STENCIL)
        .export_values();

    class_<frame_buffer_t, noncopyable>("FrameBuffer", no_init)
        .add_property("id", &frame_buffer_t::get_id)
        .add_property("color_texture", make_function(&frame_buffer_t::get_color_texture, return_value_policy<copy_const_reference>()))
        .add_property("depth_texture", make_function(&frame_buffer_t::get_depth_texture, return_value_policy<copy_const_reference>()))
        .add_property("depth_stencil_texture", make_function(&frame_buffer_t::get_depth_stencil_texture, return_value_policy<copy_const_reference>()))
        .add_property("size", make_function(&frame_buffer_t::get_size, return_value_policy<copy_const_reference>()), &frame_buffer_t::set_size)
        .add_property("type", &frame_buffer_t::get_type)
        ;

    def("bezier", mandala::bezier3<float32_t>, args("point0", "point1", "point2", "t"));

    class_<pose2>("Pose2", init<>())
        .def_readwrite("location", &pose2::location)
        .def_readwrite("rotation", &pose2::rotation)
        ;

    class_<pose3>("Pose3", init<>())
        .def_readwrite("location", &pose3::location)
        .def_readwrite("rotation", &pose3::rotation)
        ;

    enum_<actor_t::draw_type_e>("ActorDrawType")
        .value("NONE", actor_t::draw_type_e::NONE)
        .value("BSP", actor_t::draw_type_e::BSP)
        .value("MODEL", actor_t::draw_type_e::MODEL)
        .export_values();

    class_<actor_t, boost::shared_ptr<actor_t>>("Actor")
        .add_property("model", make_function(&actor_t::get_model, return_value_policy<copy_const_reference>()), &actor_t::set_model)
        .add_property("bsp", make_function(&actor_t::get_bsp, return_value_policy<copy_const_reference>()), &actor_t::set_bsp)
        .add_property("draw_type", &actor_t::get_draw_type, &actor_t::set_draw_type)
        .def_readwrite("pose", &actor_t::pose)
        ;

    class_<std::vector<boost::shared_ptr<actor_t>>>("ActorVec")
        .def(vector_indexing_suite<std::vector<boost::shared_ptr<actor_t>>>());

    class_<scene_t, noncopyable>("Scene")
        //.add_property("actors", make_function(&scene_t::get_actors, return_value_policy<copy_const_reference>()))
        .def("add_actor", &scene_t::add_actor)
        .def("render", &scene_t::render)
        .def("tick", &scene_t::tick)
        .def("on_input_event", &scene_t::on_input_event)
        ;

    {
        auto camera_scope = class_<camera_t, bases<actor_t>, boost::shared_ptr<camera_t>>("Camera")
            .def_readwrite("near", &camera_t::near)
            .def_readwrite("far", &camera_t::far)
            .def_readwrite("fov", &camera_t::fov)
            .def_readwrite("projection_type", &camera_t::projection_type)
            ;

        enum_<camera_t::projection_type_e>("ProjectionType")
            .value("ORTHOGRAPHIC", camera_t::projection_type_e::ORTHOGRAPHIC)
            .value("PERSPECTIVE", camera_t::projection_type_e::PERSPECTIVE)
            .export_values();
    }

    class_<quake_camera_t, bases<camera_t>, boost::shared_ptr<quake_camera_t>, noncopyable>("QuakeCamera", init<>());

    class_<arcball_camera_t, bases<camera_t>, boost::shared_ptr<arcball_camera_t>, noncopyable>("ArcballCamera", init<>());

    class_<model_instance_t, boost::shared_ptr<model_instance_t>, noncopyable>("ModelInstance", init<const hash_t&>());
}
