#pragma once

//boost
#include <boost/preprocessor.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

//naga
#include "game_object.hpp"
#include "game_component.hpp"
#include "app.hpp"
#include "arcball_camera.hpp"
#include "audio_mgr.hpp"
#include "audio_source.hpp"
#include "bitmap_font.hpp"
#include "bsp.hpp"
#include "cache_mgr.hpp"
#include "camera_component.hpp"
#include "closure_traits.hpp"
#include "frame_buffer.hpp"
#include "game.hpp"
#include "gui_button.hpp"
#include "gui_canvas.hpp"
#include "gui_image.hpp"
#include "gui_label.hpp"
#include "gui_layout.hpp"
#include "gui_scroll.hpp"
#include "http_response.hpp"
#include "http_request.hpp"
#include "http_manager.hpp"
#include "image.hpp"
#include "input_event.hpp"
#include "interpolation.hpp"
#include "io.hpp"
#include "material.hpp"
#include "model.hpp"
#include "model_animation.hpp"
#include "model_component.hpp"
#include "monitor.hpp"
#include "padding.hpp"
#include "physics_simulation.hpp"
#include "pose.hpp"
#include "python.hpp"
#include "python_optional.hpp"
#include "quake_camera.hpp"
#include "rectangle.hpp"
#include "resource_mgr.hpp"
#include "rigid_body_component.hpp"
#include "scene.hpp"
#include "sound.hpp"
#include "sprite_set.hpp"
#include "state.hpp"
#include "state.hpp"
#include "state_mgr.hpp"
#include "string_mgr.hpp"
#include "texture.hpp"
#include "terrain_component.hpp"
#include "python_pair.hpp"
#include "python_function_from_callable.hpp"

#include "psk.hpp"

using namespace boost;
using namespace boost::python;
using namespace naga;

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

#define NAGA_PYTHON_DECLARE_WRAPPER_CLASS(name) struct name##_wrapper : name, wrapper<name>

#define NAGA_PYTHON_DECLARE_WRAPPER_CLASS_T1(name)\
template<typename T0>\
struct name##_wrapper<T0> : name<T0>, wrapper<name>

#define NAGA_PYTHON_DEFINE_WRAPPER_FUNCTION(return_type, name, ...)\
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

#define NAGA_PYTHON_DEFINE_WRAPPER_FUNCTION_VIRTUAL(return_type, name, ...)\
return_type name(__VA_ARGS__) override\
{\
    auto override_ = get_override(BOOST_PP_STRINGIZE(name));\
    \
    if (override_)\
    {\
        override_();\
    }\
}\

NAGA_PYTHON_DECLARE_WRAPPER_CLASS(game)
{
    NAGA_PYTHON_DEFINE_WRAPPER_FUNCTION_VIRTUAL(void, on_run_start)
    NAGA_PYTHON_DEFINE_WRAPPER_FUNCTION_VIRTUAL(void, on_run_end)

    void on_tick_start(f32 dt) override
    {
        auto override_ = get_override("on_tick_start");

        if (override_)
        {
            override_(dt);
        }
    }

    void on_tick_end(f32 dt) override
    {
        auto override_ = get_override("on_tick_end");

        if (override_)
        {
            override_(dt);
        }
    }

    NAGA_PYTHON_DEFINE_WRAPPER_FUNCTION_VIRTUAL(void, on_render_start)
    NAGA_PYTHON_DEFINE_WRAPPER_FUNCTION_VIRTUAL(void, on_render_end)

    bool on_input_event(input_event_t& input_event) override
    {
        auto override_ = get_override("on_input_event");

        if (override_)
        {
            return override_(input_event);
        }

        return false;
    }

#if defined(NAGA_PC)
    void on_window_event(window_event& window_event) override
    {
        auto override_ = get_override("on_window_event");

        if (override_)
        {
            override_(window_event);
        }
    }
#endif
};

NAGA_PYTHON_DECLARE_WRAPPER_CLASS(state)
{
    void on_tick(f32 dt) override
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

    void on_tick_base(f32 dt)
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
            return state::on_input_event(input_event);
        }
    }

    bool on_input_event_base(input_event_t& input_event)
    {
        return _wrapper_wrapped_type_::on_input_event(input_event);
    }

    NAGA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, render)
    NAGA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_active)
    NAGA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_passive)
    NAGA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_enter)
    NAGA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_exit)
    NAGA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_stop_tick)
    NAGA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_start_tick)
    NAGA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_stop_render)
    NAGA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_start_render)
    NAGA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_stop_input)
    NAGA_PYTHON_DEFINE_WRAPPER_FUNCTION(void, on_start_input)
};

// GAME COMPONENT
NAGA_PYTHON_DECLARE_WRAPPER_CLASS(game_component)
{
    void on_tick(f32 dt) override
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

    void on_tick_base(f32 dt)
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
            return _wrapper_wrapped_type_::on_input_event(input_event);
        }
    }

    bool on_input_event_base(input_event_t& input_event)
    {
        return _wrapper_wrapped_type_::on_input_event(input_event);
    }
};

#define NAGA_DEFINE_RESOURCE_GET_FUNCTION(name)\
boost::shared_ptr<name> resources_get_##name(resource_mgr& resources, const std::string& hash) { return resources.get<name>(hash); }

NAGA_DEFINE_RESOURCE_GET_FUNCTION(bitmap_font)
NAGA_DEFINE_RESOURCE_GET_FUNCTION(bsp)
NAGA_DEFINE_RESOURCE_GET_FUNCTION(image)
NAGA_DEFINE_RESOURCE_GET_FUNCTION(material)
NAGA_DEFINE_RESOURCE_GET_FUNCTION(model)
NAGA_DEFINE_RESOURCE_GET_FUNCTION(sound)
NAGA_DEFINE_RESOURCE_GET_FUNCTION(sprite_set)
NAGA_DEFINE_RESOURCE_GET_FUNCTION(texture)

template<typename V>
struct vec3_wrapper : glm::detail::tvec3<V>, wrapper<glm::detail::tvec3<V>>
{
    vec3_wrapper(V x, V y, V z) :
        _wrapper_wrapped_type_(x, y, z)
    {
    }

    vec3_wrapper(const _wrapper_wrapped_type_& other) :
        _wrapper_wrapped_type_(other)
    {
    }

    f32 __length__() const
    {
        return glm::length(*reinterpret_cast<const _wrapper_wrapped_type_*>(this));
    }

    _wrapper_wrapped_type_ normalize() const
    {
        return glm::normalize(*this);
    }

    operator _wrapper_wrapped_type_()
    {
        return _wrapper_wrapped_type_(x, y, z);
    }
};

#define NAGA_PYTHON_DEFINE_VEC2(name, value_type)\
class_<glm::detail::tvec2<value_type>>(name, init<value_type, value_type>())\
    .def_readwrite("x", &glm::detail::tvec2<value_type>::x)\
    .def_readwrite("y", &glm::detail::tvec2<value_type>::y)\
    .def(self_ns::self + self_ns::self)\
    .def(self_ns::self += self_ns::self)\
    .def(self_ns::self - self_ns::self)\
    .def(self_ns::self -= self_ns::self)\
    .def(self_ns::str(self_ns::self))\
    .def(self_ns::repr(self_ns::self));

//#define NAGA_PYTHON_DEFINE_VEC3(name, V)\
//class_<glm::detail::tvec3<V>, noncopyab>(BOOST_PP_CAT(name, "Base"), no_init)\
//    .def_readwrite("x", &glm::detail::tvec3<V>::x)\
//    .def_readwrite("y", &glm::detail::tvec3<V>::y)\
//    .def_readwrite("z", &glm::detail::tvec3<V>::z)\
//    .def(self_ns::self + self_ns::self)\
//    .def(self_ns::self += self_ns::self)\
//    .def(self_ns::self - self_ns::self)\
//    .def(self_ns::self -= self_ns::self)\
//    .def(self_ns::self * 0.0f)\
//    .def(self_ns::str(self_ns::self));\
//class_<vec3_wrapper<V>, bases<glm::detail::tvec3<V>>>(name, init<V, V, V>())\
//    .def("length", &vec3_wrapper<V>::__length__);

#define NAGA_PYTHON_DEFINE_VEC3(name, V)\
class_<glm::detail::tvec3<V>>(name, init<V, V, V>())\
    .def_readwrite("x", &glm::detail::tvec3<V>::x)\
    .def_readwrite("y", &glm::detail::tvec3<V>::y)\
    .def_readwrite("z", &glm::detail::tvec3<V>::z)\
    .def(self_ns::self + self_ns::self)\
    .def(self_ns::self += self_ns::self)\
    .def(self_ns::self - self_ns::self)\
    .def(self_ns::self -= self_ns::self)\
    .def(self_ns::self * 0.0f)\
    .def(self_ns::str(self_ns::self));

#define NAGA_PYTHON_DEFINE_VEC4(name, value_type)\
class_<glm::detail::tvec4<value_type>>(name, init<value_type, value_type, value_type, value_type>())\
    .def_readwrite("x", &glm::detail::tvec4<value_type>::x)\
    .def_readwrite("y", &glm::detail::tvec4<value_type>::y)\
    .def_readwrite("z", &glm::detail::tvec4<value_type>::z)\
    .def_readwrite("w", &glm::detail::tvec4<value_type>::w)\
    .def(self_ns::self + self_ns::self)\
    .def(self_ns::self += self_ns::self)\
    .def(self_ns::self - self_ns::self)\
    .def(self_ns::self -= self_ns::self)\
    .def(self_ns::str(self_ns::self));

#define NAGA_PYTHON_DEFINE_QUAT(name, value_type)\
class_<glm::detail::tquat<value_type>>(name, init<value_type, value_type, value_type, value_type>())\
    .def_readwrite("x", &glm::detail::tquat<value_type>::x)\
    .def_readwrite("y", &glm::detail::tquat<value_type>::y)\
    .def_readwrite("z", &glm::detail::tquat<value_type>::z)\
    .def_readwrite("w", &glm::detail::tquat<value_type>::w)\
    .def("__len__", &glm::detail::tquat<value_type>::length)\
    .def(self_ns::self * self_ns::self)\
    .def(self_ns::self * glm::detail::tvec3<value_type>())\
    .def(self_ns::str(self_ns::self));

#define NAGA_PYTHON_DEFINE_AABB2(name, scalar_type)\
class_<naga::details::aabb2<scalar_type>>(name, init<>())\
    .def_readwrite("min", &naga::details::aabb2<scalar_type>::min)\
    .def_readwrite("max", &naga::details::aabb2<scalar_type>::max)\
    .add_property("width", &naga::details::aabb2<scalar_type>::width)\
    .add_property("height", &naga::details::aabb2<scalar_type>::height)\
    .add_property("size", make_function(&naga::details::aabb2<scalar_type>::size, return_value_policy<return_by_value>()))\
    .add_property("center", make_function(&naga::details::aabb2<scalar_type>::center, return_value_policy<return_by_value>()))\
    .def(self_ns::self + other<naga::details::aabb2<scalar_type>::value_type>())\
    .def(self_ns::self += other<naga::details::aabb2<scalar_type>::value_type>())\
    .def(self_ns::self - other<naga::details::aabb2<scalar_type>::value_type>())\
    .def(self_ns::self -= other<naga::details::aabb2<scalar_type>::value_type>())\
    .def(self_ns::str(self_ns::self));

#define NAGA_PYTHON_DEFINE_AABB3(name, scalar_type)\
class_<naga::details::aabb3<scalar_type>>(name, init<>())\
    .def_readwrite("min", &naga::details::aabb3<scalar_type>::min)\
    .def_readwrite("max", &naga::details::aabb3<scalar_type>::max)\
    .add_property("width", &naga::details::aabb3<scalar_type>::width)\
    .add_property("height", &naga::details::aabb3<scalar_type>::height)\
    .add_property("depth", &naga::details::aabb3<scalar_type>::depth)\
    .add_property("size", make_function(&naga::details::aabb3<scalar_type>::size, return_value_policy<return_by_value>()))\
    .add_property("center", make_function(&naga::details::aabb3<scalar_type>::center, return_value_policy<return_by_value>()))\
    .def(self_ns::self + other<naga::details::aabb3<scalar_type>::value_type>())\
    .def(self_ns::self += other<naga::details::aabb3<scalar_type>::value_type>())\
    .def(self_ns::self - other<naga::details::aabb3<scalar_type>::value_type>())\
    .def(self_ns::self -= other<naga::details::aabb3<scalar_type>::value_type>());

#define NAGA_PYTHON_DEFINE_RECTANGLE(name, scalar_type)\
class_<naga::details::rectangle<scalar_type>>(name, init<>())\
    .def_readwrite("x", &naga::details::rectangle<scalar_type>::x)\
    .def_readwrite("y", &naga::details::rectangle<scalar_type>::y)\
    .def_readwrite("width", &naga::details::rectangle<scalar_type>::width)\
    .def_readwrite("height", &naga::details::rectangle<scalar_type>::height);

#define NAGA_PYTHON_DEFINE_PADDING(name, scalar_type)\
class_<naga::details::padding<scalar_type>>(name, init<scalar_type, scalar_type, scalar_type, scalar_type>())\
    .def_readwrite("bottom", &naga::details::padding<scalar_type>::bottom)\
    .def_readwrite("left", &naga::details::padding<scalar_type>::left)\
    .def_readonly("top", &naga::details::padding<scalar_type>::top)\
    .def_readonly("right", &naga::details::padding<scalar_type>::right)\
    .add_property("vertical", &naga::details::padding<scalar_type>::vertical)\
    .add_property("horizontal", &naga::details::padding<scalar_type>::horizontal)\
    .add_property("size", &naga::details::padding<scalar_type>::size)\
    .def(self_ns::self + self_ns::self)\
    .def(self_ns::self += self_ns::self)\
    .def(self_ns::self - self_ns::self)\
    .def(self_ns::self -= self_ns::self)\
    .def(self_ns::str(self_ns::self));

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(state_mgr_push_overloads, state_mgr::push, 2, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(http_manager_get_overloads, http_manager::get, 1, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(http_manager_get_async_overloads, http_manager::get_async, 1, 2)

inline naga::quat angle_axis(f32 angle, const naga::vec3& axis)
{
    return glm::angleAxis(angle, axis);
}

inline f32 length(const boost::python::object& other)
{
    auto result = boost::python::extract<glm::detail::tvec3<f32>>(other);

    if (result.check())
    {
        return glm::length(result());
    }

    throw std::invalid_argument("length cannot be calculated");
}

inline boost::python::object inverse(const boost::python::object& other)
{
    auto result = boost::python::extract<glm::detail::tquat<f32>>(other);

    if (result.check())
    {
        return boost::python::object(glm::inverse(result()));
    }

    throw std::invalid_argument("invalid argument for 'inverse'");
}

inline boost::python::object normalize(const boost::python::object& other)
{
    auto result = boost::python::extract<glm::detail::tvec3<f32>>(other);

    if (result.check())
    {
        return boost::python::object(glm::normalize(result()));
    }

    throw std::invalid_argument("invalid argument for 'normalize'");
}

BOOST_PYTHON_MODULE(naga)
{
    def("percent_rank", &glm::percent_rank<f32>);
    def("angle_axis", &angle_axis);
    def("bezier", naga::bezier3<f32>, args("point0", "point1", "point2", "t"));
    def("length", &length);
    def("inverse", &inverse);
    def("normalize", &normalize);

    python_optional<sprite>();
    python_optional<size_t>();
	python_function_from_callable<void(boost::shared_ptr<http_response>)>();

	python_pair<std::string, std::string>();

    enable_type_object();

    class_<naga::python, noncopyable>("Python", no_init)
        .def("execute", &naga::python::exec)
        .def("exec_file", &naga::python::exec_file);

    scope().attr("py") = boost::ref(py);

    class_<game_wrapper, boost::shared_ptr<game_wrapper>, noncopyable>("Game", init<>())
        .def("on_run_start", pure_virtual(&game::on_run_start))
        .def("on_run_end", pure_virtual(&game::on_run_end))
        .def("on_tick_start", pure_virtual(&game::on_tick_start))
        .def("on_tick_end", pure_virtual(&game::on_tick_end))
        .def("on_render_start", pure_virtual(&game::on_render_start))
        .def("on_render_end", pure_virtual(&game::on_render_end))
		.def("on_input_event", pure_virtual(&game::on_input_event));

    class_<naga::hash>("Hash", init<std::string>());

    //MATH
    NAGA_PYTHON_DEFINE_AABB2("AABB2I", i32);
    NAGA_PYTHON_DEFINE_AABB2("AABB2F", f32);

    NAGA_PYTHON_DEFINE_AABB3("AABB3I", i32);
    NAGA_PYTHON_DEFINE_AABB3("AABB3F", f32);

    //move to GLM module?
    NAGA_PYTHON_DEFINE_VEC2("Vec2I", i32);
    NAGA_PYTHON_DEFINE_VEC2("Vec2F", f32);
    NAGA_PYTHON_DEFINE_VEC2("Vec2D", f64);

    //NAGA_PYTHON_DEFINE_VEC3("Vec3I", i32);
    NAGA_PYTHON_DEFINE_VEC3("Vec3F", f32);

    NAGA_PYTHON_DEFINE_VEC4("Vec4I", i32);
    NAGA_PYTHON_DEFINE_VEC4("Vec4F", f32);

    NAGA_PYTHON_DEFINE_QUAT("Quat", f32);

    NAGA_PYTHON_DEFINE_RECTANGLE("RectangleI", i32);
    NAGA_PYTHON_DEFINE_RECTANGLE("RectangleF", f32);

    NAGA_PYTHON_DEFINE_PADDING("PaddingI", i32);
    NAGA_PYTHON_DEFINE_PADDING("PaddingF", f32);

    //PLATFORM
    //TODO: remove specialized platform structs, have only 1 platform_t per platform
    {
        scope platform_scope = 
#if defined(NAGA_WINDOWS)
        class_<platform_win32, noncopyable>("Platform", no_init)
#elif defined(NAGA_OSX)
        class_<platform_osx_t, noncopyable>("Platform", no_init)
#endif
            .add_property("screen_size", &platform_t::get_screen_size, &platform_t::set_screen_size)
#if defined(NAGA_PC)
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
            ;
    }

    {
        scope input_event_scope = class_<input_event_t>("InputEvent", no_init)
            .add_property("id", &input_event_t::id)
            .add_property("device_type", &input_event_t::device_type)
            .add_property("touch", &input_event_t::touch)
            .add_property("keyboard", &input_event_t::keyboard)
#if defined(NAGA_PC)
            .add_property("gamepad", &input_event_t::gamepad)
#endif
            ;

        scope().attr("MOD_FLAG_SHIFT") = input_event_t::mod_flags_type(input_event_t::MOD_FLAG_SHIFT);
        scope().attr("MOD_FLAG_CTRL") = input_event_t::mod_flags_type(input_event_t::MOD_FLAG_CTRL);
        scope().attr("MOD_FLAG_ALT") = input_event_t::mod_flags_type(input_event_t::MOD_FLAG_ALT);
        scope().attr("MOD_FLAG_SUPER") = input_event_t::mod_flags_type(input_event_t::MOD_FLAG_SUPER);

        enum_<input_event_t::device_type_e>("DeviceType")
            .value("NONE", input_event_t::device_type_e::NONE)
            .value("TOUCH", input_event_t::device_type_e::TOUCH)
            .value("KEYBOARD", input_event_t::device_type_e::KEYBOARD)
#if defined(NAGA_PC)
            .value("GAMEPAD", input_event_t::device_type_e::GAMEPAD)
#endif
            .value("COUNT", input_event_t::device_type_e::COUNT)
            .export_values();

        {
            scope touch_scope = class_<input_event_t::touch_t, noncopyable>("Touch", no_init)
                .def_readonly("id", &input_event_t::touch_t::id)
                .def_readonly("type", &input_event_t::touch_t::type)
#if defined(NAGA_PC)
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

#if defined(NAGA_PC)
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

#if defined(NAGA_PC)
        {
            scope gamepad_scope = class_<input_event_t::gamepad_t, noncopyable>("Gamepad", no_init)
                .def_readonly("axis_index", &input_event_t::gamepad_t::axis_index)
                .def_readonly("axis_value", &input_event_t::gamepad_t::axis_value)
                .def_readonly("axis_value_delta", &input_event_t::gamepad_t::axis_value_delta)
                .def_readonly("button_index", &input_event_t::gamepad_t::button_index)
                .def_readonly("index", &input_event_t::gamepad_t::index)
                .def_readonly("type", &input_event_t::gamepad_t::type);

            enum_<input_event_t::gamepad_t::type_e>("Type")
                .value("AXIS_MOVE", input_event_t::gamepad_t::type_e::AXIS_MOVE)
                .value("NONE", input_event_t::gamepad_t::type_e::NONE)
                .value("PRESS", input_event_t::gamepad_t::type_e::PRESS)
                .value("RELEASE", input_event_t::gamepad_t::type_e::RELEASE)
                .export_values();
        }

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

#if defined(NAGA_PC)
    enum_<window_event_type>("WindowEventType")
        .value("MOVE", window_event_type::MOVE)
        .value("RESIZE", window_event_type::RESIZE)
        .export_values();

    class_<window_event>("WindowEvent", no_init)
        .add_property("type", &window_event::type)
        .add_property("rectangle", &window_event::rectangle);
#endif

    scope().attr("platform") = boost::ref(platform);

    //APP
    {
        scope app_scope = class_<app, noncopyable>("App", no_init)
            .def("exit", &app::exit)
            .def("reset", &app::reset)
            .def("run", &app::run)
            .add_property("performance", make_function(&app::get_performance, return_value_policy<copy_const_reference>()))
            .add_property("uptime", &app::get_uptime);

        class_<app::performance_t>("Performance", no_init)
            .def_readonly("fps", &app::performance_t::fps);
    }

    scope().attr("app") = boost::ref(app_);

    //STRING MANAGER
    class_<string_mgr, noncopyable>("StringMgr", no_init)
        .def("mount", &string_mgr::mount)
        .def("get", &string_mgr::get)
        .def("purge", &string_mgr::purge)
        .add_property("count", &string_mgr::count);

    scope().attr("strings") = boost::ref(strings);

    //STATE MANAGER
    scope().attr("STATE_FLAG_NONE") = state_flags_type(STATE_FLAG_NONE);
    scope().attr("STATE_FLAG_RENDER") = state_flags_type(STATE_FLAG_RENDER);
    scope().attr("STATE_FLAG_INPUT") = state_flags_type(STATE_FLAG_INPUT);
    scope().attr("STATE_FLAG_TICK") = state_flags_type(STATE_FLAG_TICK);
    scope().attr("STATE_FLAG_ALL") = state_flags_type(STATE_FLAG_ALL);

    class_<state_mgr, noncopyable>("StateMgr", no_init)
        .def("push", &state_mgr::push, state_mgr_push_overloads(boost::python::args("state", "link_flags", "weight")))
        .def("pop", &state_mgr::pop)
        .def("change_link_flags", &state_mgr::change_link_flags)
        .def("change_weight", &state_mgr::change_weight)
        .def("purge", &state_mgr::purge)
        .def("get_flags", &state_mgr::get_flags)
        .def("get_link_flags", &state_mgr::get_link_flags)
        .add_property("count", &state_mgr::count)
        .def("at", &state_mgr::at)
        .def("index_of", &state_mgr::index_of)
        .def("is_state_rendering", &state_mgr::is_state_rendering)
        .def("is_state_ticking", &state_mgr::is_state_ticking)
        .def("is_state_handling_input", &state_mgr::is_state_handling_input)
        .def("is_state_popping", &state_mgr::is_state_popping)
        .def("is_state_changing_link_flags", &state_mgr::is_state_changing_link_flags);

    scope().attr("states") = boost::ref(states);

    //RESOURCES
    class_<resource_mgr, noncopyable>("ResourceMgr", no_init)
        .def("mount", &resource_mgr::mount)
        .def("unmount_all", &resource_mgr::unmount_all)
        .def("extract", &resource_mgr::extract)
        .add_property("count", &resource_mgr::count)
        .def("purge", &resource_mgr::purge)
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
    class_<cache_mgr, noncopyable>("CacheMgr", no_init)
        .def("purge", &cache_mgr::purge)
        .def("put", &cache_mgr::put);

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

    enum_<gui_dock_mode>("GuiDockMode")
        .value("NONE", gui_dock_mode::NONE)
        .value("BOTTOM", gui_dock_mode::BOTTOM)
        .value("FILL", gui_dock_mode::FILL)
        .value("LEFT", gui_dock_mode::LEFT)
        .value("RIGHT", gui_dock_mode::RIGHT)
        .value("TOP", gui_dock_mode::TOP)
        .export_values();

    enum_<gui_visiblity>("GuiVisibility")
        .value("OMIT", gui_visiblity::OMIT)
        .value("HIDDEN", gui_visiblity::HIDDEN)
        .value("VISIBLE", gui_visiblity::VISIBLE)
        .export_values();

    class_<gui_size_modes_t>("GuiSizeModes", init<gui_size_mode_e, gui_size_mode_e>())
        .add_property("x", make_function(&gui_size_modes_t::get_x), &gui_size_modes_t::set_x)
        .add_property("y", make_function(&gui_size_modes_t::get_y), &gui_size_modes_t::set_y)
        ;

    class_<frame_buffer, boost::shared_ptr<frame_buffer>, noncopyable>("FrameBuffer", no_init);

    class_<gui_node, boost::shared_ptr<gui_node>, noncopyable>("GuiNode", init<>())
        .add_property("root", make_function(&gui_node::get_root, return_value_policy<copy_const_reference>()))
        .add_property("parent", make_function(&gui_node::get_parent, return_value_policy<copy_const_reference>()))
        .add_property("dock_mode", &gui_node::get_dock_mode, &gui_node::set_dock_mode)
        .add_property("anchor_flags", &gui_node::get_anchor_flags, &gui_node::set_anchor_flags)
        .add_property("anchor_offset", make_function(&gui_node::get_anchor_offset, return_value_policy<copy_const_reference>()), &gui_node::set_anchor_offset)
        .add_property("padding", make_function(&gui_node::get_padding, return_value_policy<copy_const_reference>()), &gui_node::set_padding)
        .add_property("margin", make_function(&gui_node::get_margin, return_value_policy<copy_const_reference>()), &gui_node::set_margin)
        .add_property("size", make_function(&gui_node::get_size, return_value_policy<copy_const_reference>()), &gui_node::set_size)
        .add_property("size_modes", make_function(&gui_node::get_size_modes, return_value_policy<copy_const_reference>()), &gui_node::set_size_modes)
        .add_property("color", make_function(&gui_node::get_color, return_value_policy<copy_const_reference>()), &gui_node::set_color)
        .add_property("bounds", make_function(&gui_node::get_bounds, return_value_policy<copy_const_reference>()), &gui_node::set_bounds)
        .add_property("should_clip", &gui_node::get_should_clip, &gui_node::set_should_clip)
        .add_property("is_dirty", &gui_node::get_is_dirty)
        .add_property("visibility", &gui_node::get_visibility, &gui_node::set_visibility)
        .add_property("has_children", &gui_node::has_children)
        .add_property("has_parent", &gui_node::has_parent)
        .def("orphan", &gui_node::orphan)
        .def("dirty", &gui_node::dirty)
        .def("adopt", &gui_node::adopt)
        ;

    {
        scope gui_button_scope = class_<gui_button, bases<gui_node>, boost::shared_ptr<gui_button>, noncopyable>("GuiButton", init<>())
            .add_property("state", &gui_button::get_state)
            .def("on_state_changed", WRAP_MEM(gui_button, on_state_changed))
            .def("set_on_state_changed", SET_MEM(gui_button, on_state_changed))
            ;

        enum_<gui_button::state_t>("State")
            .value("IDLE", gui_button::state_t::IDLE)
#if defined(NAGA_PC)
            .value("HOVER", gui_button::state_t::HOVER)
#endif
            .value("PRESSED", gui_button::state_t::PRESSED)
            .export_values();
    }

    class_<gui_canvas, bases<gui_node>, boost::shared_ptr<gui_canvas>, noncopyable>("GuiCanvas", init<>())
        .add_property("frame_buffer", make_function(&gui_canvas::get_frame_buffer, return_value_policy<copy_const_reference>()));

    {
        scope gui_label_scope = class_<gui_label, bases<gui_node>, boost::shared_ptr<gui_label>, noncopyable>("GuiLabel", init<>())
            .add_property("bitmap_font", make_function(&gui_label::get_bitmap_font, return_value_policy<copy_const_reference>()), &gui_label::set_bitmap_font)
            .add_property("string", make_function(&gui_label::get_string, return_value_policy<copy_const_reference>()), &gui_label::set_string)
            .add_property("justification", &gui_label::get_justification, &gui_label::set_justification)
            .add_property("vertical_alignment", &gui_label::get_vertical_alignment, &gui_label::set_vertical_alignment)
            .add_property("line_spacing", &gui_label::get_line_spacing, &gui_label::set_line_spacing)
            .add_property("is_multiline", &gui_label::get_is_multiline, &gui_label::set_is_multiline)
            .add_property("should_use_ellipses", &gui_label::get_should_use_ellipses, &gui_label::set_should_use_ellipses)
            .add_property("should_use_color_codes", &gui_label::get_should_use_color_codes, &gui_label::set_should_use_color_codes)
            .add_property("is_read_only", &gui_label::get_is_read_only, &gui_label::set_is_read_only)
            .add_property("max_length", &gui_label::get_max_length, &gui_label::set_max_length)
            .add_property("is_autosized_to_text", &gui_label::get_is_autosized_to_text, &gui_label::set_is_autosized_to_text)
            .add_property("is_obscured", &gui_label::get_is_obscured, &gui_label::set_is_obscured)
            .def("escape_string", &gui_label::escape_string, return_value_policy<copy_non_const_reference>())
            .staticmethod("escape_string");


        enum_<gui_label::justification_e>("Justification")
            .value("LEFT", gui_label::justification_e::LEFT)
            .value("CENTER", gui_label::justification_e::CENTER)
            .value("RIGHT", gui_label::justification_e::RIGHT)
            .export_values();

        enum_<gui_label::vertical_alignment_e>("VerticalAlignment")
            .value("TOP", gui_label::vertical_alignment_e::TOP)
            .value("MIDDLE", gui_label::vertical_alignment_e::MIDDLE)
            .value("BOTTOM", gui_label::vertical_alignment_e::BOTTOM)
            .export_values();
    }
    
    {
        scope gui_image_scope = class_<gui_image, bases<gui_node>, boost::shared_ptr<gui_image>, noncopyable>("GuiImage", init<>())
            .add_property("sprite", make_function(&gui_image::get_sprite, return_value_policy<copy_const_reference>()), &gui_image::set_sprite)
            .add_property("is_autosized_to_sprite", &gui_image::get_is_autosized_to_sprite, &gui_image::set_is_autosized_to_sprite)
            .add_property("triangle_mode", &gui_image::get_triangle_mode, &gui_image::set_triangle_mode)
            .add_property("texcoord_scale", make_function(&gui_image::get_texcoord_scale, return_value_policy<copy_const_reference>()), &gui_image::set_texcoord_scale)
            .add_property("texcoord_origin", make_function(&gui_image::get_texcoord_origin, return_value_policy<copy_const_reference>()), &gui_image::set_texcoord_origin)
            .add_property("texcoord_rotation", &gui_image::get_texcoord_rotation, &gui_image::set_texcoord_rotation)
            .add_property("slice_padding", make_function(&gui_image::get_slice_padding, return_value_policy<copy_const_reference>()), &gui_image::set_slice_padding);

        enum_<gui_image::triangle_mode_e>("TriangleMode")
            .value("BOTTOM_RIGHT", gui_image::triangle_mode_e::BOTTOM_RIGHT)
            .value("TOP_LEFT", gui_image::triangle_mode_e::TOP_LEFT)
            .value("TOP_RIGHT", gui_image::triangle_mode_e::TOP_RIGHT)
            .value("BOTTOM_LEFT", gui_image::triangle_mode_e::BOTTOM_LEFT)
            .value("BOTH", gui_image::triangle_mode_e::BOTH)
            .value("SLICE", gui_image::triangle_mode_e::SLICE);
    }

    class_<gui_scroll, bases<gui_node>, boost::shared_ptr<gui_scroll>, noncopyable>("GuiScroll")
        .add_property("scroll_extents", make_getter(&gui_scroll::scroll_extents, return_value_policy<copy_non_const_reference>()), make_setter(&gui_scroll::scroll_extents))
        .def("set_scroll_location", &gui_scroll::set_scroll_location);

    class_<gui_layout, bases<gui_node>, boost::shared_ptr<gui_layout>, noncopyable>("GuiLayout", no_init);

    //STATES
    class_<state, boost::shared_ptr<state>, noncopyable>("StateBase", no_init);

    class_<state_wrapper, boost::shared_ptr<state_wrapper>, noncopyable>("State", init<>())
        .def("tick", &state_wrapper::tick)
        .def("on_input_event", &state_wrapper::on_input_event)
        .def("on_input_event_base", &state_wrapper::on_input_event_base)
#if defined(NAGA_PC)
        .def("on_window_event", &state_wrapper::on_window_event)
#endif
        .def("on_active", &state_wrapper::on_active)
        .def("on_passive", &state_wrapper::on_passive)
        .def("on_enter", &state_wrapper::on_enter)
        .def("on_exit", &state_wrapper::on_exit)
        .def("on_stop_tick", &state_wrapper::on_stop_tick)
        .def("on_start_tick", &state_wrapper::on_start_tick)
        .def("on_stop_render", &state_wrapper::on_stop_render)
        .def("on_start_render", &state_wrapper::on_start_render)
        .def("on_stop_input", &state_wrapper::on_stop_input)
        .def("on_start_input", &state_wrapper::on_start_input)
        .def("render_base", &state_wrapper::render_base)
        .add_property("layout", make_function(&state_wrapper::get_layout, return_value_policy<copy_const_reference>()));

    //RESOURCES
    class_<resource, boost::shared_ptr<resource>, noncopyable>("Resource", no_init);

    class_<bitmap_font, bases<resource>, boost::shared_ptr<bitmap_font>, noncopyable>("BitmapFont", no_init)
        .add_property("size", &bitmap_font::get_size)
        .add_property("is_smooth", &bitmap_font::get_is_smooth)
        .add_property("is_unicode", &bitmap_font::get_is_unicode)
        .add_property("is_bold", &bitmap_font::get_is_bold)
        .add_property("is_fixed_height", &bitmap_font::get_is_fixed_height)
        .add_property("char_set", &bitmap_font::get_char_set)
        .add_property("stretch_height", &bitmap_font::get_stretch_height)
        .add_property("antialiasing", &bitmap_font::get_antialiasing)
        .add_property("padding", make_function(&bitmap_font::get_padding, return_value_policy<copy_const_reference>()))
        .add_property("spacing_horizontal", &bitmap_font::get_spacing_horizontal)
        .add_property("spacing_vertical", &bitmap_font::get_spacing_vertical)
        .add_property("outline", &bitmap_font::get_outline)
        .add_property("line_height", &bitmap_font::get_line_height)
        .add_property("base", &bitmap_font::get_base)
        .add_property("width", &bitmap_font::get_width)
        .add_property("height", &bitmap_font::get_height)
        .add_property("page_count", &bitmap_font::get_page_count)
        .add_property("flags_1", &bitmap_font::get_flags_1)
        .add_property("alpha_channel", &bitmap_font::get_alpha_channel)
        .add_property("red_channel", &bitmap_font::get_red_channel)
        .add_property("green_channel", &bitmap_font::get_green_channel)
        .add_property("blue_channel", &bitmap_font::get_blue_channel);

    class_<std::map<const naga::hash, boost::shared_ptr<sprite_set::region>>>("SpriteSetRegionMap")
        .def(map_indexing_suite<std::map<const naga::hash, boost::shared_ptr<sprite_set::region>>>());

    class_<texture, bases<resource>, boost::shared_ptr<texture>, noncopyable>("Texture", no_init)
        .add_property("size", make_function(&texture::get_size, return_value_policy<copy_const_reference>()))
        .add_property("id", &texture::get_id);

    class_<image, bases<resource>, boost::shared_ptr<image>, noncopyable>("Image", no_init)
        .add_property("bit_depth", &image::get_bit_depth)
        //.add_property("color_type", &image::get_color_type)
        .add_property("width", &image::get_width)
        .add_property("height", &image::get_height)
        .add_property("channel_count", &image::get_channel_count);

    class_<sprite_set::region, boost::shared_ptr<sprite_set::region>, noncopyable>("SpriteSetRegion", no_init)
        .add_property("hash", &sprite_set::region::hash)
        .add_property("frame_rectangle", &sprite_set::region::frame_rectangle)
        .add_property("rectangle", &sprite_set::region::rectangle)
        .add_property("source_size", &sprite_set::region::source_size)
        .add_property("is_rotated", &sprite_set::region::is_rotated)
        .add_property("is_trimmed", &sprite_set::region::is_trimmed)
        .add_property("frame_uv", &sprite_set::region::frame_uv)
        .add_property("uv", &sprite_set::region::uv)
        ;

    {
        scope sprite_set_scope = class_<sprite_set, bases<resource>, boost::shared_ptr<sprite_set>, noncopyable>("SpriteSet", no_init)
            .add_property("texture", make_function(&sprite_set::get_texture, return_value_policy<copy_const_reference>()))
            .add_property("regions", make_function(&sprite_set::get_regions, return_value_policy<copy_const_reference>()))
            ;
    }

    class_<sprite, noncopyable>("Sprite", init<const std::string&, const naga::hash&>())
        .add_property("region", make_function(&sprite::get_region, return_value_policy<copy_const_reference>()))
        .add_property("sprite_set", make_function(&sprite::get_sprite_set, return_value_policy<copy_const_reference>()));

    class_<std::vector<const std::string>>("StringVec")
        .def(vector_indexing_suite<std::vector<const std::string>>());

    class_<model, bases<resource>, boost::shared_ptr<model>, noncopyable>("Model", no_init)
        .add_property("bones", make_function(&model::get_bone_names, return_value_policy<copy_const_reference>()));

    class_<bsp, bases<resource>, boost::shared_ptr<bsp>, noncopyable>("Bsp", no_init);

    //GPU
    class_<gpu_t, noncopyable>("Gpu", no_init)
        .add_property("vendor", make_function(&gpu_t::get_vendor, return_value_policy<copy_const_reference>()))
        .add_property("renderer", make_function(&gpu_t::get_renderer, return_value_policy<copy_const_reference>()))
        .add_property("shading_language_version", make_function(&gpu_t::get_shading_language_version, return_value_policy<copy_const_reference>()))
        .add_property("version", make_function(&gpu_t::get_version, return_value_policy<copy_const_reference>()))
        .add_property("extensions", make_function(&gpu_t::get_extensions, return_value_policy<copy_const_reference>()))
        .add_property("clear_color", make_function(&gpu_t::get_clear_color, return_value_policy<return_by_value>()), &gpu_t::set_clear_color);

    scope().attr("gpu") = boost::ref(gpu);

    enum_<gpu_frame_buffer_type>("FrameBufferType")
        .value("COLOR", gpu_frame_buffer_type::COLOR)
        .value("COLOR_DEPTH", gpu_frame_buffer_type::COLOR_DEPTH)
        .value("COLOR_DEPTH_STENCIL", gpu_frame_buffer_type::COLOR_DEPTH_STENCIL)
        .value("DEPTH", gpu_frame_buffer_type::DEPTH)
        .value("DEPTH_STENCIL", gpu_frame_buffer_type::DEPTH_STENCIL)
        .export_values();

    class_<frame_buffer, noncopyable>("FrameBuffer", no_init)
        .add_property("id", &frame_buffer::get_id)
        .add_property("color_texture", make_function(&frame_buffer::get_color_texture, return_value_policy<copy_const_reference>()))
        .add_property("depth_texture", make_function(&frame_buffer::get_depth_texture, return_value_policy<copy_const_reference>()))
        .add_property("depth_stencil_texture", make_function(&frame_buffer::get_depth_stencil_texture, return_value_policy<copy_const_reference>()))
        .add_property("size", make_function(&frame_buffer::get_size, return_value_policy<copy_const_reference>()), &frame_buffer::set_size)
        .add_property("type", &frame_buffer::get_type)
        ;

    class_<pose2>("Pose2")
        .def_readwrite("location", &pose2::location)
        .def_readwrite("rotation", &pose2::rotation)
        ;

    class_<pose3>("Pose3")
        .def_readwrite("location", &pose3::location)
        .def_readwrite("rotation", &pose3::rotation)
        ;

    class_<game_object, boost::shared_ptr<game_object>, noncopyable>("GameObject")
        .def_readwrite("pose", &game_object::pose)
        .def("add_component", &game_object::add_component_by_type)
        .def("add_component_by_name", &game_object::add_component_by_name)
        .def("get_component", &game_object::get_component_by_type)
        ;

    // COMPONENTS
    class_<game_component, boost::shared_ptr<game_component>>("GameComponentBase", no_init)
        .add_property("owner", make_function(&game_component::get_owner, return_value_policy<copy_const_reference>()))
        ;

    class_<game_component_wrapper, boost::shared_ptr<game_component_wrapper>, noncopyable>("GameComponent", init<>())
        .def("on_tick", &game_component_wrapper::on_tick)
        .def("on_input_event", &game_component_wrapper::on_input_event)
        .def("on_input_event_base", &game_component_wrapper::on_input_event_base)
        .add_property("owner", make_function(&game_component_wrapper::get_owner, return_value_policy<copy_const_reference>()));

    class_<std::vector<boost::shared_ptr<game_object>>>("GameObjectVec")
        .def(vector_indexing_suite<std::vector<boost::shared_ptr<game_object>>>());

    class_<scene, boost::shared_ptr<scene>, noncopyable>("Scene")
        .add_property("physics", make_function(&scene::get_physics, return_value_policy<copy_const_reference>()))
        .def("create_game_object", &scene::create_game_object)
        .def("remove_game_object", &scene::remove_game_object)
        .def("render", &scene::render)
        .def("tick", &scene::tick)
        .def("on_input_event", &scene::on_input_event)
        ;

    {
        auto camera_scope = class_<camera_component, bases<game_component>, boost::shared_ptr<camera_component>>(camera_component::component_name, init<>())
            .def_readwrite("near", &camera_component::near)
            .def_readwrite("far", &camera_component::far)
            .def_readwrite("fov", &camera_component::fov)
            .def_readwrite("projection_type", &camera_component::projection_type)
            ;

        enum_<camera_component::projection_type_e>("ProjectionType")
            .value("ORTHOGRAPHIC", camera_component::projection_type_e::ORTHOGRAPHIC)
            .value("PERSPECTIVE", camera_component::projection_type_e::PERSPECTIVE)
            .export_values();
    }

    class_<sound, bases<resource>, boost::shared_ptr<sound>, noncopyable>("Sound", no_init)
        .add_property("channel_count", &sound::get_channel_count)
        .add_property("sample_rate", &sound::get_sample_rate)
        .add_property("byte_rate", &sound::get_byte_rate)
        .add_property("bits_per_sample", &sound::get_bits_per_sample)
        .add_property("duration", &sound::get_duration);

    class_<model_component, bases<game_component>, boost::shared_ptr<model_component>, noncopyable>("ModelComponent")
        .add_property("model", make_function(&model_component::get_model, return_value_policy<copy_const_reference>()), &model_component::set_model)
        .def("play", &model_component::play);

    class_<model_animation, boost::shared_ptr<model_animation>, noncopyable>("ModelAnimation", no_init);

    // AUDIO
    class_<audio_mgr, noncopyable>("AudioMgr", no_init);

    class_<audio_source, boost::shared_ptr<audio_source>, noncopyable>("AudioSource", no_init);
        //.add_property("state", &audio_source::get_state);
        //.add_property("location", &audio_source::get_location)
        //;

    // PHYSICS
    class_<physics_simulation, boost::shared_ptr<physics_simulation>, noncopyable>("PhysicsSimulation")
        .add_property("gravity", &physics_simulation::get_gravity, &physics_simulation::set_gravity);

    class_<rigid_body_component, bases<game_component>, boost::shared_ptr<rigid_body_component>, noncopyable>(rigid_body_component::component_name)
        .add_property("mass", &rigid_body_component::get_mass, &rigid_body_component::set_mass)
        .add_property("center_of_mass", &rigid_body_component::get_center_of_mass, &rigid_body_component::set_center_of_mass)
        .add_property("aabb", &rigid_body_component::get_aabb);

    // TERRAIN
    class_<terrain_component, bases<game_component>, boost::shared_ptr<terrain_component>, noncopyable>(terrain_component::component_name)
		.add_property("scale", &terrain_component::get_scale, &terrain_component::set_scale)
        .def("set_heightmap", &terrain_component::set_heightmap);

	// PSK (TEMP)
	class_<PSK, boost::shared_ptr<PSK>, noncopyable>("PSK", init<const std::string&>());

    // HTTP
	enum_<http_method>("HttpMethod")
		.value("GET", http_method::GET)
		.value("POST", http_method::POST)
		.value("PUT", http_method::PUT)
		.value("DELETE", http_method::DELETE_)
		.value("HEAD", http_method::HEAD)
		.value("OPTIONS", http_method::OPTIONS)
		.export_values();

	class_<http_request, boost::shared_ptr<http_request>, noncopyable>("HttpRequest", no_init)
		.add_property("url", make_function(&http_request::get_url, return_value_policy<copy_const_reference>()))
		.add_property("method", &http_request::get_method)
		.add_property("headers", make_function(&http_request::get_headers, return_value_policy<copy_const_reference>()))
		;
	;

    class_<http_response, boost::shared_ptr<http_response>, noncopyable>("HttpResponse", no_init)
        .add_property("content", make_function(&http_response::get_content, return_value_policy<copy_const_reference>()))
        .add_property("content_type", make_function(&http_response::get_content_type, return_value_policy<copy_const_reference>()))
        .add_property("has_error", &http_response::has_error)
		.add_property("elapsed", &http_response::get_elapsed)
		;
	;

	// TODO: would be nice to just make a converter instead of having to give this a name
	// and legitimize it as an instantiable type.
	class_<std::vector<std::pair<std::string, std::string>>>("StringPairList")
		.def(vector_indexing_suite<std::vector<std::pair<std::string, std::string>>>());

    class_<http_manager, noncopyable>("HttpManager", no_init)
		.def("get", &http_manager::get,
		http_manager_get_overloads((
				boost::python::arg("url")
				//boost::python::arg("headers") = http_headers_type(),
				//boost::python::arg("data") = http_data_type(),
				//boost::python::arg("on_write") = make_function<void(size_t)>(nullptr)
			))
			)
        .def("get_async", &http_manager::get_async, 
			http_manager_get_async_overloads(
				(
					boost::python::arg("url"),
					//boost::python::arg("headers") = http_headers_type(),
					//boost::python::arg("data") = http_data_type(),
					boost::python::arg("on_response") = boost::python::make_function<void(boost::shared_ptr<http_response>)>(nullptr)
					//boost::python::arg("on_write") = make_function<void(size_t)>(nullptr)
				)
			)
		);

    scope().attr("http") = boost::ref(http);
}
