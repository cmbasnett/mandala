#pragma once

//boost
#include <boost/preprocessor.hpp>
#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

//naga
#include "game_object.hpp"
#include "game_component.hpp"
#include "app.hpp"
#include "arcball_camera.hpp"
#include "audio_system.hpp"
#include "audio_source.hpp"
#include "bitmap_font.hpp"
#include "bsp.hpp"
#include "cache.hpp"
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
#include "resource_manager.hpp"
#include "rigid_body_component.hpp"
#include "scene.hpp"
#include "sound.hpp"
#include "sprite_set.hpp"
#include "state.hpp"
#include "state.hpp"
#include "state_system.hpp"
#include "string_mgr.hpp"
#include "texture.hpp"
#include "terrain_component.hpp"
#include "python_pair.hpp"
#include "python_function_from_callable.hpp"
#include "line.hpp"
#include "psk.hpp"
#include "convex.hpp"
#include "py_boost_function.hpp"

using namespace boost;
using namespace boost::python;
using namespace naga;

//generate "type _#"
#define PARAMS(z,n,data) BOOST_PP_TUPLE_ELEM(n,data) _##n

//The first variant: with parameters
//parameters: PARAMS(z,0,(char,short,int)), PARAMS(z,1,(char,short,int)), PARAMS(z,2,(char,short,int))
//call: _0, _1, _2

#define DEFINE_FUNCTION_WRAPPER_WITH_PARAMS(return_type, name, ...)\
return_type name##Wrapper(BOOST_PP_ENUM(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), PARAMS, (__VA_ARGS__)))\
{\
    return name(BOOST_PP_ENUM_PARAMS(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), _));\
}

//The second variant: no parameters
#define DEFINE_FUNCTION_WRAPPER_WITHOUT_PARAMS(return_type, name)\
return_type name##Wrapper()\
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

#define NAGA_PYTHON_DECLARE_WRAPPER_CLASS(name) struct name##Wrapper : name, wrapper<name>

#define NAGA_PYTHON_DECLARE_WRAPPER_CLASS_T1(name)\
template<typename T0>\
struct nameWrapper<T0> : name<T0>, wrapper<name>

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

NAGA_PYTHON_DECLARE_WRAPPER_CLASS(Game)
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

    bool on_input_event(InputEvent& input_event) override
    {
        auto override_ = get_override("on_input_event");

        if (override_)
        {
            return override_(input_event);
        }

        return false;
    }

#if defined(NAGA_PC)
    void on_window_event(WindowEvent& window_event) override
    {
        auto override_ = get_override("on_window_event");

        if (override_)
        {
            override_(window_event);
        }
    }
#endif
};

NAGA_PYTHON_DECLARE_WRAPPER_CLASS(State)
{
	void render() override
	{
		auto override_ = get_override("render");

		if (override_)
		{
			override_();
		}
		else
		{
			_wrapper_wrapped_type_::render();
		}
	}

	void render_base()
	{
		_wrapper_wrapped_type_::render();
	}

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

	bool on_input_event(InputEvent& input_event) override
    {
        auto override_ = get_override("on_input_event");

        if (override_)
        {
            return override_(input_event);
        }
        else
        {
            return State::on_input_event(input_event);
        }
    }

    bool on_input_event_base(InputEvent& input_event)
    {
        return _wrapper_wrapped_type_::on_input_event(input_event);
    }

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
NAGA_PYTHON_DECLARE_WRAPPER_CLASS(GameComponent)
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

	bool on_input_event(InputEvent& input_event) override
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

    bool on_input_event_base(InputEvent& input_event)
    {
        return _wrapper_wrapped_type_::on_input_event(input_event);
    }

	void on_render_base(CameraParameters& camera_parameters)
	{
		_wrapper_wrapped_type_::on_render(camera_parameters);
	}

	void on_render(CameraParameters& camera_parameters)
	{
		auto override_ = get_override("on_render");

		if (override_)
		{
			override_(camera_parameters);
		}
		else
		{
			_wrapper_wrapped_type_::on_render(camera_parameters);
		}
	}
};

template<typename T>
inline std::vector<T> to_std_vector(boost::python::object& iterable)
{
	return std::vector<T>(boost::python::stl_input_iterator<T>(iterable), boost::python::stl_input_iterator<T>());
};

template<typename T>
boost::python::list vector_to_list(const std::vector<T>& vector)
{
	boost::python::list list;

	for (auto& v : vector)
	{
		list.append(v);
	}

	return list;
}

// TODO: this would be kinda annoying
boost::python::list Py_convex_hull(boost::python::list& object)
{
	return vector_to_list(get_convex_hull(to_std_vector<naga::vec2>(object)));
}

// TODO: this could probably be simplified to a templated function accepting a type, no?
#define NAGA_DEFINE_RESOURCE_GET_FUNCTION(type)\
boost::shared_ptr<type> resources_get_##type(ResourceManager& resources, const std::string& name) { return resources.get<type>(name); }

#define NAGA_DEFINE_RESOURCE_PUT_FUNCTION(type)\
boost::shared_ptr<type> resources_make_##type(ResourceManager& resources, const std::string& name) { return resources.make<type>(name); }

NAGA_DEFINE_RESOURCE_GET_FUNCTION(BitmapFont)
NAGA_DEFINE_RESOURCE_GET_FUNCTION(BSP)
NAGA_DEFINE_RESOURCE_GET_FUNCTION(Image)
NAGA_DEFINE_RESOURCE_GET_FUNCTION(Material)
NAGA_DEFINE_RESOURCE_GET_FUNCTION(Model)
NAGA_DEFINE_RESOURCE_GET_FUNCTION(Sound)
NAGA_DEFINE_RESOURCE_GET_FUNCTION(SpriteSet)
NAGA_DEFINE_RESOURCE_GET_FUNCTION(Texture)

NAGA_DEFINE_RESOURCE_PUT_FUNCTION(SpriteSet)

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

#define NAGA_PYTHON_DEFINE_LINE2(name, V)\
class_<naga::details::Line2<V>>(name, init<naga::details::Line2<V>::VectorType, naga::details::Line2<V>::VectorType>())\
	.def_readwrite("start", &naga::details::Line2<V>::start)\
	.def_readwrite("end", &naga::details::Line2<V>::end);

#define NAGA_PYTHON_DEFINE_LINE3(name, V)\
class_<naga::details::Line3<V>>(name, init<naga::details::Line3<V>::VectorType, naga::details::Line3<V>::VectorType>())\
	.def_readwrite("start", &naga::details::Line3<V>::start)\
	.def_readwrite("end", &naga::details::Line3<V>::end);

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
class_<naga::details::AABB2<scalar_type>>(name, init<>())\
    .def_readwrite("min", &naga::details::AABB2<scalar_type>::min)\
    .def_readwrite("max", &naga::details::AABB2<scalar_type>::max)\
    .add_property("width", &naga::details::AABB2<scalar_type>::width)\
    .add_property("height", &naga::details::AABB2<scalar_type>::height)\
    .add_property("size", make_function(&naga::details::AABB2<scalar_type>::size, return_value_policy<return_by_value>()))\
    .add_property("center", make_function(&naga::details::AABB2<scalar_type>::center, return_value_policy<return_by_value>()))\
    .def(self_ns::self + other<naga::details::AABB2<scalar_type>::VectorType>())\
    .def(self_ns::self += other<naga::details::AABB2<scalar_type>::VectorType>())\
    .def(self_ns::self - other<naga::details::AABB2<scalar_type>::VectorType>())\
    .def(self_ns::self -= other<naga::details::AABB2<scalar_type>::VectorType>())\
    .def(self_ns::str(self_ns::self));

#define NAGA_PYTHON_DEFINE_AABB3(name, scalar_type)\
class_<naga::details::AABB3<scalar_type>>(name, init<>())\
    .def_readwrite("min", &naga::details::AABB3<scalar_type>::min)\
    .def_readwrite("max", &naga::details::AABB3<scalar_type>::max)\
    .add_property("width", &naga::details::AABB3<scalar_type>::width)\
    .add_property("height", &naga::details::AABB3<scalar_type>::height)\
    .add_property("depth", &naga::details::AABB3<scalar_type>::depth)\
    .add_property("size", make_function(&naga::details::AABB3<scalar_type>::size, return_value_policy<return_by_value>()))\
    .add_property("center", make_function(&naga::details::AABB3<scalar_type>::center, return_value_policy<return_by_value>()))\
    .def(self_ns::self + other<naga::details::AABB3<scalar_type>::VectorType>())\
    .def(self_ns::self += other<naga::details::AABB3<scalar_type>::VectorType>())\
    .def(self_ns::self - other<naga::details::AABB3<scalar_type>::VectorType>())\
    .def(self_ns::self -= other<naga::details::AABB3<scalar_type>::VectorType>())\
    .def(self_ns::str(self_ns::self));;

#define NAGA_PYTHON_DEFINE_RECTANGLE(name, scalar_type)\
class_<naga::details::Rectangle<scalar_type>>(name, init<>())\
    .def_readwrite("x", &naga::details::Rectangle<scalar_type>::x)\
    .def_readwrite("y", &naga::details::Rectangle<scalar_type>::y)\
    .def_readwrite("width", &naga::details::Rectangle<scalar_type>::width)\
    .def_readwrite("height", &naga::details::Rectangle<scalar_type>::height);

#define NAGA_PYTHON_DEFINE_PADDING(name, scalar_type)\
class_<naga::details::Padding<scalar_type>>(name, init<scalar_type, scalar_type, scalar_type, scalar_type>())\
    .def_readwrite("bottom", &naga::details::Padding<scalar_type>::bottom)\
    .def_readwrite("left", &naga::details::Padding<scalar_type>::left)\
    .def_readonly("top", &naga::details::Padding<scalar_type>::top)\
    .def_readonly("right", &naga::details::Padding<scalar_type>::right)\
    .add_property("vertical", &naga::details::Padding<scalar_type>::vertical)\
    .add_property("horizontal", &naga::details::Padding<scalar_type>::horizontal)\
    .add_property("size", &naga::details::Padding<scalar_type>::size)\
    .def(self_ns::self + self_ns::self)\
    .def(self_ns::self += self_ns::self)\
    .def(self_ns::self - self_ns::self)\
    .def(self_ns::self -= self_ns::self)\
    .def(self_ns::str(self_ns::self));

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(state_mgr_push_overloads, StateSystem::push, 2, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(http_manager_get_overloads, HttpManager::get, 1, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(http_manager_get_async_overloads, HttpManager::get_async, 1, 2)

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

inline boost::python::object degrees(const boost::python::object& radians)
{
	auto result = boost::python::extract<f32>(radians);

	if (result.check())
	{
		return boost::python::object(glm::degrees(result()));
	}

	throw std::invalid_argument("invalid argument for 'degrees'");
}

inline boost::python::object radians(const boost::python::object& degrees)
{
	auto result = boost::python::extract<f32>(degrees);

	if (result.check())
	{
		return boost::python::object(glm::radians(result()));
	}

	throw std::invalid_argument("invalid argument for 'degrees'");
}

BOOST_PYTHON_MODULE(naga)
{
    def("percent_rank", &glm::percent_rank<f32>);
    def("angle_axis", &angle_axis);
    def("bezier", naga::bezier3<f32>, args("point0", "point1", "point2", "t"));
    def("length", &length);
    def("inverse", &inverse);
    def("normalize", &normalize);
	def("radians", &radians);
	def("degrees", &degrees);

    python_optional<Sprite>();
    python_optional<size_t>();
	python_function_from_callable<void(boost::shared_ptr<HttpResponse>)>();
	python_function_from_callable<bool(boost::shared_ptr<GUINode>)>();

	python_pair<std::string, std::string>();

    enable_type_object();

    class_<Python, noncopyable>("Python", no_init)
		.def("execute", &Python::exec)
		.def("exec_file", &Python::exec_file);

    scope().attr("py") = boost::ref(py);

    class_<GameWrapper, boost::shared_ptr<GameWrapper>, noncopyable>("Game", init<>())
        .def("on_run_start", pure_virtual(&Game::on_run_start))
		.def("on_run_end", pure_virtual(&Game::on_run_end))
		.def("on_tick_start", pure_virtual(&Game::on_tick_start))
		.def("on_tick_end", pure_virtual(&Game::on_tick_end))
		.def("on_render_start", pure_virtual(&Game::on_render_start))
		.def("on_render_end", pure_virtual(&Game::on_render_end))
		.def("on_input_event", pure_virtual(&Game::on_input_event));

    //MATH
    NAGA_PYTHON_DEFINE_AABB2("AABB2", f32);
    NAGA_PYTHON_DEFINE_AABB3("AABB3", f32);

    //move to GLM module?
    NAGA_PYTHON_DEFINE_VEC2("Vec2", f32);
    NAGA_PYTHON_DEFINE_VEC3("Vec3", f32);
    NAGA_PYTHON_DEFINE_VEC4("Vec4", f32);

	NAGA_PYTHON_DEFINE_LINE2("Line2", f32);
	NAGA_PYTHON_DEFINE_LINE3("Line3", f32);

    NAGA_PYTHON_DEFINE_QUAT("Quat", f32);

    NAGA_PYTHON_DEFINE_RECTANGLE("Rectangle", f32);

    NAGA_PYTHON_DEFINE_PADDING("Padding", f32);

    //PLATFORM
    {
        scope platform_scope =  class_<Platform, noncopyable>("Platform", no_init)
			.add_property("screen_size", &Platform::get_screen_size, &Platform::set_screen_size)
			.add_property("viewport", &Platform::get_viewport)
	#if defined(NAGA_PC)
			.add_property("cursor_location", &Platform::get_cursor_location, &Platform::set_cursor_location)
			.add_property("window_size", &Platform::get_window_size, &Platform::set_window_size)
			.add_property("window_location", &Platform::get_window_location, &Platform::set_window_location)
				//.add_property("display", &platform_t::get_display, &platform_t::set_window_location)
				.add_property("fullscreen", &Platform::is_fullscreen, &Platform::set_is_fullscreen)
				.def_readwrite("is_cursor_centered", &Platform::is_cursor_centered)
				.add_property("is_cursor_hidden", &Platform::is_cursor_hidden, &Platform::set_cursor_hidden)
	#endif
				.add_property("clipboard_string", &Platform::get_clipboard_string, &Platform::set_clipboard_string)
				;

			class_<Platform::Display, noncopyable>("PlatformDisplay", no_init)
				.def_readonly("name", &Platform::Display::name)
				.def_readonly("position", &Platform::Display::position)
				.def_readonly("ppi", &Platform::Display::ppi)
				;
    }

    {
		scope input_event_scope = class_<InputEvent>("InputEvent", no_init)
            .add_property("id", &InputEvent::id)
            .add_property("device_type", &InputEvent::device_type)
            .add_property("touch", &InputEvent::touch)
            .add_property("keyboard", &InputEvent::keyboard)
#if defined(NAGA_PC)
            .add_property("gamepad", &InputEvent::gamepad)
#endif
            ;

        scope().attr("MOD_FLAG_SHIFT") = InputEvent::ModFlagsType(InputEvent::MOD_FLAG_SHIFT);
		scope().attr("MOD_FLAG_CTRL") = InputEvent::ModFlagsType(InputEvent::MOD_FLAG_CTRL);
		scope().attr("MOD_FLAG_ALT") = InputEvent::ModFlagsType(InputEvent::MOD_FLAG_ALT);
		scope().attr("MOD_FLAG_SUPER") = InputEvent::ModFlagsType(InputEvent::MOD_FLAG_SUPER);

		enum_<InputEvent::DeviceType>("DeviceType")
			.value("NONE", InputEvent::DeviceType::NONE)
			.value("TOUCH", InputEvent::DeviceType::TOUCH)
			.value("KEYBOARD", InputEvent::DeviceType::KEYBOARD)
#if defined(NAGA_PC)
			.value("GAMEPAD", InputEvent::DeviceType::GAMEPAD)
#endif
			.value("COUNT", InputEvent::DeviceType::COUNT)
            .export_values();

        {
			scope touch_scope = class_<InputEvent::Touch, noncopyable>("Touch", no_init)
				.def_readonly("id", &InputEvent::Touch::id)
				.def_readonly("type", &InputEvent::Touch::type)
#if defined(NAGA_PC)
				.def_readonly("button", &InputEvent::Touch::button)
#endif
				.def_readonly("location", &InputEvent::Touch::location)
				.def_readonly("location_delta", &InputEvent::Touch::location_delta)
				.def_readonly("mod_flags", &InputEvent::Touch::mod_flags);

			enum_<InputEvent::Touch::Type>("Type")
				.value("NONE", InputEvent::Touch::Type::NONE)
				.value("PRESS", InputEvent::Touch::Type::PRESS)
				.value("RELEASE", InputEvent::Touch::Type::RELEASE)
				.value("SCROLL", InputEvent::Touch::Type::SCROLL)
				.value("MOVE", InputEvent::Touch::Type::MOVE)
                .export_values();

#if defined(NAGA_PC)
			enum_<InputEvent::Touch::Button>("Button")
				.value("NONE", InputEvent::Touch::Button::NONE)
				.value("LEFT", InputEvent::Touch::Button::LEFT)
				.value("RIGHT", InputEvent::Touch::Button::RIGHT)
				.value("MIDDLE", InputEvent::Touch::Button::MIDDLE)
				.value("FOUR", InputEvent::Touch::Button::FOUR)
				.value("FIVE", InputEvent::Touch::Button::FIVE)
				.value("SIX", InputEvent::Touch::Button::SIX)
				.value("SEVEN", InputEvent::Touch::Button::SEVEN)
				.value("EIGHT", InputEvent::Touch::Button::EIGHT)
				.value("COUNT", InputEvent::Touch::Button::COUNT)
                .export_values();
#endif
        }

#if defined(NAGA_PC)
        {
			scope gamepad_scope = class_<InputEvent::Gamepad, noncopyable>("Gamepad", no_init)
				.def_readonly("axis_index", &InputEvent::Gamepad::axis_index)
				.def_readonly("axis_value", &InputEvent::Gamepad::axis_value)
				.def_readonly("axis_value_delta", &InputEvent::Gamepad::axis_value_delta)
				.def_readonly("button_index", &InputEvent::Gamepad::button_index)
				.def_readonly("index", &InputEvent::Gamepad::index)
				.def_readonly("type", &InputEvent::Gamepad::type);

			enum_<InputEvent::Gamepad::Type>("Type")
				.value("AXIS_MOVE", InputEvent::Gamepad::Type::AXIS_MOVE)
				.value("NONE", InputEvent::Gamepad::Type::NONE)
				.value("PRESS", InputEvent::Gamepad::Type::PRESS)
				.value("RELEASE", InputEvent::Gamepad::Type::RELEASE)
                .export_values();
        }

        {
			scope keyboard_scope = class_<InputEvent::Keyboard, noncopyable>("Keyboard", no_init)
				.def_readonly("type", &InputEvent::Keyboard::type)
				.def_readonly("key", &InputEvent::Keyboard::key)
				.def_readonly("mod_flags", &InputEvent::Keyboard::mod_flags)
				.def_readonly("character", &InputEvent::Keyboard::character);

			enum_<InputEvent::Keyboard::Type>("Type")
				.value("NONE", InputEvent::Keyboard::Type::NONE)
				.value("KEY_RELEASE", InputEvent::Keyboard::Type::KEY_RELEASE)
				.value("KEY_PRESS", InputEvent::Keyboard::Type::KEY_PRESS)
				.value("KEY_REPEAT", InputEvent::Keyboard::Type::KEY_REPEAT)
				.value("CHARACTER", InputEvent::Keyboard::Type::CHARACTER)
                .export_values();

			enum_<InputEvent::Keyboard::Key>("Key")
				.value("NONE", InputEvent::Keyboard::Key::NONE)
				.value("SPACE", InputEvent::Keyboard::Key::SPACE)
				.value("APOSTROPHE", InputEvent::Keyboard::Key::APOSTROPHE)
                .value("COMMA", InputEvent::Keyboard::Key::COMMA)
                .value("MINUS", InputEvent::Keyboard::Key::MINUS)
                .value("PERIOD", InputEvent::Keyboard::Key::PERIOD)
                .value("SLASH", InputEvent::Keyboard::Key::SLASH)
                .value("NR_0", InputEvent::Keyboard::Key::NR_0)
                .value("NR_1", InputEvent::Keyboard::Key::NR_1)
                .value("NR_2", InputEvent::Keyboard::Key::NR_2)
                .value("NR_3", InputEvent::Keyboard::Key::NR_3)
                .value("NR_4", InputEvent::Keyboard::Key::NR_4)
                .value("NR_5", InputEvent::Keyboard::Key::NR_5)
                .value("NR_6", InputEvent::Keyboard::Key::NR_6)
                .value("NR_7", InputEvent::Keyboard::Key::NR_7)
                .value("NR_8", InputEvent::Keyboard::Key::NR_8)
                .value("NR_9", InputEvent::Keyboard::Key::NR_9)
                .value("SEMICOLON", InputEvent::Keyboard::Key::SEMICOLON)
                .value("EQUAL", InputEvent::Keyboard::Key::EQUAL)
                .value("A", InputEvent::Keyboard::Key::A)
                .value("B", InputEvent::Keyboard::Key::B)
                .value("C", InputEvent::Keyboard::Key::C)
                .value("D", InputEvent::Keyboard::Key::D)
                .value("E", InputEvent::Keyboard::Key::E)
                .value("F", InputEvent::Keyboard::Key::F)
                .value("G", InputEvent::Keyboard::Key::G)
                .value("H", InputEvent::Keyboard::Key::H)
                .value("I", InputEvent::Keyboard::Key::I)
                .value("J", InputEvent::Keyboard::Key::J)
                .value("K", InputEvent::Keyboard::Key::K)
                .value("L", InputEvent::Keyboard::Key::L)
                .value("M", InputEvent::Keyboard::Key::M)
                .value("N", InputEvent::Keyboard::Key::N)
                .value("O", InputEvent::Keyboard::Key::O)
                .value("P", InputEvent::Keyboard::Key::P)
                .value("Q", InputEvent::Keyboard::Key::Q)
                .value("R", InputEvent::Keyboard::Key::R)
                .value("S", InputEvent::Keyboard::Key::S)
                .value("T", InputEvent::Keyboard::Key::T)
                .value("U", InputEvent::Keyboard::Key::U)
                .value("V", InputEvent::Keyboard::Key::V)
                .value("W", InputEvent::Keyboard::Key::W)
                .value("X", InputEvent::Keyboard::Key::X)
                .value("Y", InputEvent::Keyboard::Key::Y)
                .value("Z", InputEvent::Keyboard::Key::Z)
                .value("LEFT_BRACKET", InputEvent::Keyboard::Key::LEFT_BRACKET)
                .value("BACKSLASH", InputEvent::Keyboard::Key::BACKSLASH)
                .value("RIGHT_BRACKET", InputEvent::Keyboard::Key::RIGHT_BRACKET)
                .value("GRAVE_ACCENT", InputEvent::Keyboard::Key::GRAVE_ACCENT)
                .value("WORLD_1", InputEvent::Keyboard::Key::WORLD_1)
                .value("WORLD_2", InputEvent::Keyboard::Key::WORLD_2)
                .value("ESCAPE", InputEvent::Keyboard::Key::ESCAPE)
                .value("ENTER", InputEvent::Keyboard::Key::ENTER)
                .value("TAB", InputEvent::Keyboard::Key::TAB)
                .value("BACKSPACE", InputEvent::Keyboard::Key::BACKSPACE)
                .value("INSERT", InputEvent::Keyboard::Key::INSERT)
                .value("DEL", InputEvent::Keyboard::Key::DEL)
                .value("RIGHT", InputEvent::Keyboard::Key::RIGHT)
                .value("LEFT", InputEvent::Keyboard::Key::LEFT)
                .value("DOWN", InputEvent::Keyboard::Key::DOWN)
                .value("UP", InputEvent::Keyboard::Key::UP)
                .value("PAGE_UP", InputEvent::Keyboard::Key::PAGE_UP)
                .value("PAGE_DOWN", InputEvent::Keyboard::Key::PAGE_DOWN)
                .value("HOME", InputEvent::Keyboard::Key::HOME)
                .value("END", InputEvent::Keyboard::Key::END)
                .value("CAPS_LOCK", InputEvent::Keyboard::Key::CAPS_LOCK)
                .value("SCROLL_LOCK", InputEvent::Keyboard::Key::SCROLL_LOCK)
                .value("NUM_LOCK", InputEvent::Keyboard::Key::NUM_LOCK)
                .value("PRINT_SCREEN", InputEvent::Keyboard::Key::PRINT_SCREEN)
                .value("PAUSE", InputEvent::Keyboard::Key::PAUSE)
                .value("F1", InputEvent::Keyboard::Key::F1)
                .value("F2", InputEvent::Keyboard::Key::F2)
                .value("F3", InputEvent::Keyboard::Key::F3)
                .value("F4", InputEvent::Keyboard::Key::F4)
                .value("F5", InputEvent::Keyboard::Key::F5)
                .value("F6", InputEvent::Keyboard::Key::F6)
                .value("F7", InputEvent::Keyboard::Key::F7)
                .value("F8", InputEvent::Keyboard::Key::F8)
                .value("F9", InputEvent::Keyboard::Key::F9)
                .value("F10", InputEvent::Keyboard::Key::F10)
                .value("F11", InputEvent::Keyboard::Key::F11)
                .value("F12", InputEvent::Keyboard::Key::F12)
                .value("F13", InputEvent::Keyboard::Key::F13)
                .value("F14", InputEvent::Keyboard::Key::F14)
                .value("F15", InputEvent::Keyboard::Key::F15)
                .value("F16", InputEvent::Keyboard::Key::F16)
                .value("F17", InputEvent::Keyboard::Key::F17)
                .value("F18", InputEvent::Keyboard::Key::F18)
                .value("F19", InputEvent::Keyboard::Key::F19)
                .value("F20", InputEvent::Keyboard::Key::F20)
                .value("F21", InputEvent::Keyboard::Key::F21)
                .value("F22", InputEvent::Keyboard::Key::F22)
                .value("F23", InputEvent::Keyboard::Key::F23)
                .value("F24", InputEvent::Keyboard::Key::F24)
                .value("F25", InputEvent::Keyboard::Key::F25)
                .value("KP_0", InputEvent::Keyboard::Key::KP_0)
                .value("KP_1", InputEvent::Keyboard::Key::KP_1)
                .value("KP_2", InputEvent::Keyboard::Key::KP_2)
                .value("KP_3", InputEvent::Keyboard::Key::KP_3)
                .value("KP_4", InputEvent::Keyboard::Key::KP_4)
                .value("KP_5", InputEvent::Keyboard::Key::KP_5)
                .value("KP_6", InputEvent::Keyboard::Key::KP_6)
                .value("KP_7", InputEvent::Keyboard::Key::KP_7)
                .value("KP_8", InputEvent::Keyboard::Key::KP_8)
                .value("KP_9", InputEvent::Keyboard::Key::KP_9)
                .value("KP_DECIMAL", InputEvent::Keyboard::Key::KP_DECIMAL)
                .value("KP_DIVIDE", InputEvent::Keyboard::Key::KP_DIVIDE)
                .value("KP_MULTIPLY", InputEvent::Keyboard::Key::KP_MULTIPLY)
                .value("KP_SUBTRACT", InputEvent::Keyboard::Key::KP_SUBTRACT)
                .value("KP_ADD", InputEvent::Keyboard::Key::KP_ADD)
                .value("KP_ENTER", InputEvent::Keyboard::Key::KP_ENTER)
                .value("KP_EQUAL", InputEvent::Keyboard::Key::KP_EQUAL)
                .value("LEFT_SHIFT", InputEvent::Keyboard::Key::LEFT_SHIFT)
                .value("LEFT_CONTROL", InputEvent::Keyboard::Key::LEFT_CONTROL)
                .value("LEFT_ALT", InputEvent::Keyboard::Key::LEFT_ALT)
                .value("LEFT_SUPER", InputEvent::Keyboard::Key::LEFT_SUPER)
                .value("RIGHT_SHIFT", InputEvent::Keyboard::Key::RIGHT_SHIFT)
                .value("RIGHT_CONTROL", InputEvent::Keyboard::Key::RIGHT_CONTROL)
                .value("RIGHT_ALT", InputEvent::Keyboard::Key::RIGHT_ALT)
                .value("RIGHT_SUPER", InputEvent::Keyboard::Key::RIGHT_SUPER)
                .value("MENU", InputEvent::Keyboard::Key::MENU)
                .export_values();
        }
#endif
    }

#if defined(NAGA_PC)
    enum_<WindowEventType>("WindowEventType")
		.value("MOVE", WindowEventType::MOVE)
		.value("RESIZE", WindowEventType::RESIZE)
        .export_values();

    class_<WindowEvent>("WindowEvent", no_init)
		.add_property("type", &WindowEvent::type)
		.add_property("rectangle", &WindowEvent::rectangle);
#endif

    scope().attr("platform") = boost::ref(platform);

    //APP
    {
        scope app_scope = class_<App, noncopyable>("App", no_init)
			.def("exit", &App::exit)
			.def("reset", &App::reset)
			.def("run", &App::run)
			.def("screenshot", &App::screenshot)
			.add_property("performance", make_function(&App::get_performance, return_value_policy<copy_const_reference>()))
			.add_property("uptime_seconds", &App::get_uptime_seconds)
			.add_property("uptime_milliseconds", &App::get_uptime_milliseconds)
			;

        class_<App::Performance>("Performance", no_init)
			.def_readonly("fps", &App::Performance::fps);
    }

    scope().attr("app") = boost::ref(app);

    //STRING MANAGER
    class_<StringManager, noncopyable>("StringManager", no_init)
		.def("mount", &StringManager::mount)
		.def("get", &StringManager::get)
		.def("purge", &StringManager::purge)
		.add_property("count", &StringManager::count);

    scope().attr("strings") = boost::ref(strings);

    //STATE MANAGER
    scope().attr("STATE_FLAG_NONE") = StateFlagsType(STATE_FLAG_NONE);
	scope().attr("STATE_FLAG_RENDER") = StateFlagsType(STATE_FLAG_RENDER);
	scope().attr("STATE_FLAG_INPUT") = StateFlagsType(STATE_FLAG_INPUT);
	scope().attr("STATE_FLAG_TICK") = StateFlagsType(STATE_FLAG_TICK);
	scope().attr("STATE_FLAG_ALL") = StateFlagsType(STATE_FLAG_ALL);

    class_<StateSystem, noncopyable>("StateSystem", no_init)
		.def("push", &StateSystem::push, state_mgr_push_overloads(boost::python::args("state", "link_flags", "weight")))
		.def("pop", &StateSystem::pop)
		.def("change_link_flags", &StateSystem::change_link_flags)
		.def("change_weight", &StateSystem::change_weight)
		.def("purge", &StateSystem::purge)
		.def("get_flags", &StateSystem::get_flags)
		.def("get_link_flags", &StateSystem::get_link_flags)
		.add_property("count", &StateSystem::count)
		.def("at", &StateSystem::at)
		.def("contains", &StateSystem::contains)
		.def("index_of", &StateSystem::index_of)
		.def("is_state_rendering", &StateSystem::is_state_rendering)
		.def("is_state_ticking", &StateSystem::is_state_ticking)
		.def("is_state_handling_input", &StateSystem::is_state_handling_input)
		.def("is_state_popping", &StateSystem::is_state_popping)
		.def("is_state_changing_link_flags", &StateSystem::is_state_changing_link_flags);

    scope().attr("states") = boost::ref(states);

    //RESOURCES
    class_<ResourceManager, noncopyable>("ResourceManager", no_init)
		.def("mount", &ResourceManager::mount)
		.def("unmount_all", &ResourceManager::unmount_all)
		.def("extract", &ResourceManager::extract)
		.add_property("count", &ResourceManager::count)
		.def("purge", &ResourceManager::purge)
        .def("get_bitmap_font", &resources_get_BitmapFont)
        .def("get_bsp", &resources_get_BSP)
        .def("get_image", &resources_get_Image)
        .def("get_material", &resources_get_Material)
        .def("get_model", &resources_get_Model)
        .def("get_sound", &resources_get_Sound)
        .def("get_sprite_set", &resources_get_SpriteSet)
        .def("get_texture", &resources_get_Texture)
        ;

    scope().attr("resources") = boost::ref(resources);

    //CACHE
    class_<Cache, noncopyable>("Cache", no_init)
		.def("purge", &Cache::purge)
		.def("put", &Cache::put);

    scope().attr("cache") = boost::ref(cache);

    //GUI
    scope().attr("GUI_ANCHOR_FLAG_NONE") = GUIAnchorFlagsType(GUI_ANCHOR_FLAG_NONE);
    scope().attr("GUI_ANCHOR_FLAG_BOTTOM") = GUIAnchorFlagsType(GUI_ANCHOR_FLAG_BOTTOM);
    scope().attr("GUI_ANCHOR_FLAG_LEFT") = GUIAnchorFlagsType(GUI_ANCHOR_FLAG_LEFT);
    scope().attr("GUI_ANCHOR_FLAG_RIGHT") = GUIAnchorFlagsType(GUI_ANCHOR_FLAG_RIGHT);
    scope().attr("GUI_ANCHOR_FLAG_TOP") = GUIAnchorFlagsType(GUI_ANCHOR_FLAG_TOP);
    scope().attr("GUI_ANCHOR_FLAG_VERTICAL") = GUIAnchorFlagsType(GUI_ANCHOR_FLAG_VERTICAL);
    scope().attr("GUI_ANCHOR_FLAG_HORIZONTAL") = GUIAnchorFlagsType(GUI_ANCHOR_FLAG_HORIZONTAL);
    scope().attr("GUI_ANCHOR_FLAG_ALL") = GUIAnchorFlagsType(GUI_ANCHOR_FLAG_ALL);

    enum_<GUISizeMode>("GuiSizeMode")
        .value("ABSOLUTE", GUISizeMode::ABSOLUTE)
        .value("AXIS_SCALE", GUISizeMode::AXIS_SCALE)
        .value("INHERIT", GUISizeMode::INHERIT)
        .value("RELATIVE", GUISizeMode::RELATIVE)
        .export_values();

    enum_<GUIDockMode>("GuiDockMode")
        .value("NONE", GUIDockMode::NONE)
        .value("BOTTOM", GUIDockMode::BOTTOM)
        .value("FILL", GUIDockMode::FILL)
        .value("LEFT", GUIDockMode::LEFT)
        .value("RIGHT", GUIDockMode::RIGHT)
        .value("TOP", GUIDockMode::TOP)
        .export_values();

    enum_<GUIVisibility>("GuiVisibility")
        .value("OMIT", GUIVisibility::OMIT)
        .value("HIDDEN", GUIVisibility::HIDDEN)
        .value("VISIBLE", GUIVisibility::VISIBLE)
        .export_values();

    class_<GUISizeModes>("GuiSizeModes", init<GUISizeMode, GUISizeMode>())
        .add_property("x", make_function(&GUISizeModes::get_x), &GUISizeModes::set_x)
        .add_property("y", make_function(&GUISizeModes::get_y), &GUISizeModes::set_y)
        ;

	class_<FrameBuffer, boost::shared_ptr<FrameBuffer>, noncopyable>("FrameBuffer", no_init);

	class_<GUINode, boost::shared_ptr<GUINode>, noncopyable>("GuiNode", init<>())
        .add_property("root", make_function(&GUINode::get_root, return_value_policy<copy_const_reference>()))
        .add_property("parent", make_function(&GUINode::get_parent, return_value_policy<copy_const_reference>()))
        .add_property("dock_mode", &GUINode::get_dock_mode, &GUINode::set_dock_mode)
        .add_property("anchor_flags", &GUINode::get_anchor_flags, &GUINode::set_anchor_flags)
        .add_property("anchor_offset", make_function(&GUINode::get_anchor_offset, return_value_policy<copy_const_reference>()), &GUINode::set_anchor_offset)
        .add_property("padding", make_function(&GUINode::get_padding, return_value_policy<copy_const_reference>()), &GUINode::set_padding)
        .add_property("margin", make_function(&GUINode::get_margin, return_value_policy<copy_const_reference>()), &GUINode::set_margin)
        .add_property("size", make_function(&GUINode::get_size, return_value_policy<copy_const_reference>()), &GUINode::set_size)
        .add_property("size_modes", make_function(&GUINode::get_size_modes, return_value_policy<copy_const_reference>()), &GUINode::set_size_modes)
        .add_property("color", make_function(&GUINode::get_color, return_value_policy<copy_const_reference>()), &GUINode::set_color)
        .add_property("bounds", make_function(&GUINode::get_bounds, return_value_policy<copy_const_reference>()), &GUINode::set_bounds)
        .add_property("should_clip", &GUINode::get_should_clip, &GUINode::set_should_clip)
        .add_property("is_dirty", &GUINode::get_is_dirty)
        .add_property("visibility", &GUINode::get_visibility, &GUINode::set_visibility)
        .add_property("has_children", &GUINode::has_children)
        .add_property("has_parent", &GUINode::has_parent)
        .def("orphan", &GUINode::orphan)
        .def("dirty", &GUINode::dirty)
        .def("adopt", &GUINode::adopt)
        ;

    {
		scope gui_button_scope = class_<GUIButton, bases<GUINode>, boost::shared_ptr<GUIButton>, noncopyable>("GUIButton", init<>())
			.add_property("state", &GUIButton::get_state)
			.def("on_state_changed", WRAP_MEM(GUIButton, on_state_changed))
			.def("set_on_state_changed", SET_MEM(GUIButton, on_state_changed))
            ;

		enum_<GUIButton::State>("State")
			.value("IDLE", GUIButton::State::IDLE)
#if defined(NAGA_PC)
			.value("HOVER", GUIButton::State::HOVER)
#endif
			.value("PRESSED", GUIButton::State::PRESSED)
            .export_values();
    }

	class_<GUICanvas, bases<GUINode>, boost::shared_ptr<GUICanvas>, noncopyable>("GUICanvas", init<>())
        .add_property("frame_buffer", make_function(&GUICanvas::get_frame_buffer, return_value_policy<copy_const_reference>()));

    {
		scope gui_label_scope = class_<GUILabel, bases<GUINode>, boost::shared_ptr<GUILabel>, noncopyable>("GuiLabel", init<>())
			.add_property("bitmap_font", make_function(&GUILabel::get_bitmap_font, return_value_policy<copy_const_reference>()), &GUILabel::set_bitmap_font)
			.add_property("string", make_function(&GUILabel::get_string, return_value_policy<copy_const_reference>()), &GUILabel::set_string)
			.add_property("justification", &GUILabel::get_justification, &GUILabel::set_justification)
			.add_property("vertical_alignment", &GUILabel::get_vertical_alignment, &GUILabel::set_vertical_alignment)
			.add_property("line_spacing", &GUILabel::get_line_spacing, &GUILabel::set_line_spacing)
			.add_property("is_multiline", &GUILabel::get_is_multiline, &GUILabel::set_is_multiline)
			.add_property("should_use_ellipses", &GUILabel::get_should_use_ellipses, &GUILabel::set_should_use_ellipses)
			.add_property("should_use_color_codes", &GUILabel::get_should_use_color_codes, &GUILabel::set_should_use_color_codes)
			.add_property("is_read_only", &GUILabel::get_is_read_only, &GUILabel::set_is_read_only)
			.add_property("max_length", &GUILabel::get_max_length, &GUILabel::set_max_length)
			.add_property("is_autosized_to_text", &GUILabel::get_is_autosized_to_text, &GUILabel::set_is_autosized_to_text)
			.add_property("is_obscured", &GUILabel::get_is_obscured, &GUILabel::set_is_obscured)
			.add_property("on_enter_function", &GUILabel::get_on_enter_function, &GUILabel::set_on_enter_function)
			.def("escape_string", &GUILabel::escape_string, return_value_policy<copy_non_const_reference>())
            .staticmethod("escape_string");

		def_function<bool(boost::shared_ptr<GUINode>)>("on_enter_fn", "okay");

		enum_<GUILabel::Justification>("Justification")
			.value("LEFT", GUILabel::Justification::LEFT)
			.value("CENTER", GUILabel::Justification::CENTER)
			.value("RIGHT", GUILabel::Justification::RIGHT)
            .export_values();

        enum_<GUILabel::VerticalAlignment>("VerticalAlignment")
			.value("TOP", GUILabel::VerticalAlignment::TOP)
			.value("MIDDLE", GUILabel::VerticalAlignment::MIDDLE)
			.value("BOTTOM", GUILabel::VerticalAlignment::BOTTOM)
            .export_values();
    }
    
    {
		scope gui_image_scope = class_<GUIImage, bases<GUINode>, boost::shared_ptr<GUIImage>, noncopyable>("GUIImage", init<>())
			.add_property("sprite", make_function(&GUIImage::get_sprite, return_value_policy<copy_const_reference>()), &GUIImage::set_sprite)
			.add_property("is_autosized_to_sprite", &GUIImage::get_is_autosized_to_sprite, &GUIImage::set_is_autosized_to_sprite)
			.add_property("triangle_mode", &GUIImage::get_triangle_mode, &GUIImage::set_triangle_mode)
			.add_property("texcoord_scale", make_function(&GUIImage::get_texcoord_scale, return_value_policy<copy_const_reference>()), &GUIImage::set_texcoord_scale)
			.add_property("texcoord_origin", make_function(&GUIImage::get_texcoord_origin, return_value_policy<copy_const_reference>()), &GUIImage::set_texcoord_origin)
			.add_property("texcoord_rotation", &GUIImage::get_texcoord_rotation, &GUIImage::set_texcoord_rotation)
			.add_property("slice_padding", make_function(&GUIImage::get_slice_padding, return_value_policy<copy_const_reference>()), &GUIImage::set_slice_padding);

		enum_<GUIImage::TriangleMode>("TriangleMode")
			.value("BOTTOM_RIGHT", GUIImage::TriangleMode::BOTTOM_RIGHT)
			.value("TOP_LEFT", GUIImage::TriangleMode::TOP_LEFT)
			.value("TOP_RIGHT", GUIImage::TriangleMode::TOP_RIGHT)
			.value("BOTTOM_LEFT", GUIImage::TriangleMode::BOTTOM_LEFT)
			.value("BOTH", GUIImage::TriangleMode::BOTH)
			.value("SLICE", GUIImage::TriangleMode::SLICE);
    }

	class_<GUIScroll, bases<GUINode>, boost::shared_ptr<GUIScroll>, noncopyable>("GUIScroll")
		.add_property("scroll_extents", make_getter(&GUIScroll::scroll_extents, return_value_policy<copy_non_const_reference>()), make_setter(&GUIScroll::scroll_extents))
        .def("set_scroll_location", &GUIScroll::set_scroll_location);

	class_<GUILayout, bases<GUINode>, boost::shared_ptr<GUILayout>, noncopyable>("GUILayout", no_init);

    //STATES
	class_<State, boost::shared_ptr<State>, noncopyable>("StateBase", no_init);

	class_<StateWrapper, boost::shared_ptr<StateWrapper>, noncopyable>("State", init<>())
		.def("on_tick", &StateWrapper::on_tick)
		.def("on_input_event", &StateWrapper::on_input_event)
		.def("on_input_event_base", &StateWrapper::on_input_event_base)
#if defined(NAGA_PC)
		.def("on_window_event", &StateWrapper::on_window_event)
#endif
		.def("on_active", &StateWrapper::on_active)
		.def("on_passive", &StateWrapper::on_passive)
		.def("on_enter", &StateWrapper::on_enter)
		.def("on_enter_base", &StateWrapper::on_enter_base)
		.def("on_exit", &StateWrapper::on_exit)
		.def("on_stop_tick", &StateWrapper::on_stop_tick)
		.def("on_start_tick", &StateWrapper::on_start_tick)
		.def("on_stop_render", &StateWrapper::on_stop_render)
		.def("on_start_render", &StateWrapper::on_start_render)
		.def("on_stop_input", &StateWrapper::on_stop_input)
		.def("on_start_input", &StateWrapper::on_start_input)
		.def("render", &StateWrapper::render)
		.def("render_base", &StateWrapper::render_base)
		.add_property("layout", make_function(&StateWrapper::get_layout, return_value_policy<copy_const_reference>()));

    //RESOURCES
	class_<Resource, boost::shared_ptr<Resource>, noncopyable>("Resource", no_init);

	class_<BitmapFont, bases<Resource>, boost::shared_ptr<BitmapFont>, noncopyable>("BitmapFont", no_init)
		.add_property("size", &BitmapFont::get_size)
		.add_property("is_smooth", &BitmapFont::get_is_smooth)
		.add_property("is_unicode", &BitmapFont::get_is_unicode)
		.add_property("is_bold", &BitmapFont::get_is_bold)
		.add_property("is_fixed_height", &BitmapFont::get_is_fixed_height)
		.add_property("char_set", &BitmapFont::get_char_set)
		.add_property("stretch_height", &BitmapFont::get_stretch_height)
		.add_property("antialiasing", &BitmapFont::get_antialiasing)
		.add_property("padding", make_function(&BitmapFont::get_padding, return_value_policy<copy_const_reference>()))
		.add_property("spacing_horizontal", &BitmapFont::get_spacing_horizontal)
		.add_property("spacing_vertical", &BitmapFont::get_spacing_vertical)
		.add_property("outline", &BitmapFont::get_outline)
		.add_property("line_height", &BitmapFont::get_line_height)
		.add_property("base", &BitmapFont::get_base)
		.add_property("width", &BitmapFont::get_width)
		.add_property("height", &BitmapFont::get_height)
		.add_property("page_count", &BitmapFont::get_page_count)
		.add_property("flags_1", &BitmapFont::get_flags_1)
		.add_property("alpha_channel", &BitmapFont::get_alpha_channel)
		.add_property("red_channel", &BitmapFont::get_red_channel)
		.add_property("green_channel", &BitmapFont::get_green_channel)
		.add_property("blue_channel", &BitmapFont::get_blue_channel);

	// TODO: make a macro for this!
    class_<std::map<const std::string, SpriteSet::Region>>("SpriteSetRegionMap")
		.def(map_indexing_suite<std::map<const std::string, SpriteSet::Region>>());

	class_<Texture, bases<Resource>, boost::shared_ptr<Texture>, noncopyable>("Texture", no_init)
		.add_property("size", make_function(&Texture::get_size, return_value_policy<copy_const_reference>()))
		.add_property("id", &Texture::get_id);

    class_<Image, bases<Resource>, boost::shared_ptr<Image>, noncopyable>("Image", no_init)
		.add_property("bit_depth", &Image::get_bit_depth)
        //.add_property("color_type", &image::get_color_type)
		.add_property("width", &Image::get_width)
		.add_property("height", &Image::get_height)
		.add_property("channel_count", &Image::get_channel_count);

    {
		scope sprite_set_scope = class_<SpriteSet, bases<Resource>, boost::shared_ptr<SpriteSet>, noncopyable>("SpriteSet", init<const std::string&>())
			.add_property("texture", make_function(&SpriteSet::get_texture, return_value_policy<copy_const_reference>()))
			.add_property("regions", make_function(&SpriteSet::get_regions, return_value_policy<copy_const_reference>()))
            ;

		class_<SpriteSet::Region>("Region", no_init)
			.add_property("name", &SpriteSet::Region::name)
			.add_property("frame_rectangle", &SpriteSet::Region::frame_rectangle)
			.add_property("rectangle", &SpriteSet::Region::rectangle)
			.add_property("source_size", &SpriteSet::Region::source_size)
			.add_property("is_rotated", &SpriteSet::Region::is_rotated)
			.add_property("is_trimmed", &SpriteSet::Region::is_trimmed)
			.add_property("frame_uv", &SpriteSet::Region::frame_uv)
			.add_property("uv", &SpriteSet::Region::uv)
			;
    }

	class_<Sprite, noncopyable>("Sprite", init<const boost::shared_ptr<Texture>&>())
		.add_property("region", make_function(&Sprite::get_region, return_value_policy<copy_const_reference>()))
		.add_property("texture", make_function(&Sprite::get_texture, return_value_policy<copy_const_reference>()));

	class_<std::vector<naga::vec2>>("Vec2List")
		.def(vector_indexing_suite<std::vector<naga::vec2>>());

    class_<std::vector<const std::string>>("StringList")
        .def(vector_indexing_suite<std::vector<const std::string>>());

	class_<Model, bases<Resource>, boost::shared_ptr<Model>, noncopyable>("Model", no_init)
		.add_property("bones", make_function(&Model::get_bone_names, return_value_policy<copy_const_reference>()));

    class_<BSP, bases<Resource>, boost::shared_ptr<BSP>, noncopyable>("BSP", no_init);

    //GPU
    class_<Gpu, noncopyable>("Gpu", no_init)
        .add_property("vendor", make_function(&Gpu::get_vendor, return_value_policy<copy_const_reference>()))
        .add_property("renderer", make_function(&Gpu::get_renderer, return_value_policy<copy_const_reference>()))
        .add_property("shading_language_version", make_function(&Gpu::get_shading_language_version, return_value_policy<copy_const_reference>()))
        .add_property("version", make_function(&Gpu::get_version, return_value_policy<copy_const_reference>()))
        .add_property("extensions", make_function(&Gpu::get_extensions, return_value_policy<copy_const_reference>()))
        .add_property("clear_color", make_function(&Gpu::get_clear_color, return_value_policy<return_by_value>()), &Gpu::set_clear_color);

    scope().attr("gpu") = boost::ref(gpu);

    enum_<GpuFrameBufferType>("FrameBufferType")
        .value("COLOR", GpuFrameBufferType::COLOR)
        .value("COLOR_DEPTH", GpuFrameBufferType::COLOR_DEPTH)
        .value("COLOR_DEPTH_STENCIL", GpuFrameBufferType::COLOR_DEPTH_STENCIL)
        .value("DEPTH", GpuFrameBufferType::DEPTH)
        .value("DEPTH_STENCIL", GpuFrameBufferType::DEPTH_STENCIL)
        .export_values();

    class_<FrameBuffer, noncopyable>("FrameBuffer", no_init)
		.add_property("id", &FrameBuffer::get_id)
		.add_property("color_texture", make_function(&FrameBuffer::get_color_texture, return_value_policy<copy_const_reference>()))
		.add_property("depth_texture", make_function(&FrameBuffer::get_depth_texture, return_value_policy<copy_const_reference>()))
		.add_property("depth_stencil_texture", make_function(&FrameBuffer::get_depth_stencil_texture, return_value_policy<copy_const_reference>()))
		.add_property("size", make_function(&FrameBuffer::get_size, return_value_policy<copy_const_reference>()), &FrameBuffer::set_size)
		.add_property("type", &FrameBuffer::get_type)
        ;

	class_<TraceResult>("TraceResult")
		.def_readonly("did_hit", &TraceResult::did_hit)
		.def_readonly("location", &TraceResult::location)
		.def_readonly("normal", &TraceResult::normal)
		;


    class_<Pose2>("Pose2")
        .def_readwrite("location", &Pose2::location)
		.def_readwrite("rotation", &Pose2::rotation)
		.def(self_ns::str(self_ns::self));
        ;

    class_<Pose3>("Pose3")
		.def_readwrite("location", &Pose3::location)
		.def_readwrite("rotation", &Pose3::rotation)
		.def(self_ns::str(self_ns::self));
        ;

	class_<GameObject, boost::shared_ptr<GameObject>, noncopyable>("GameObject")
        .def_readwrite("pose", &GameObject::pose)
		.def("add_component", &GameObject::add_component_by_type)
		.def("add_component_by_name", &GameObject::add_component_by_name)
		.def("get_component", &GameObject::get_component_by_type)
        ;

    // COMPONENTS
	class_<GameComponent, boost::shared_ptr<GameComponent>>("GameComponentBase", no_init)
        .add_property("owner", make_function(&GameComponent::get_owner, return_value_policy<copy_const_reference>()))
        ;

	class_<GameComponentWrapper, boost::shared_ptr<GameComponentWrapper>, noncopyable>("GameComponent", init<>())
		.def("on_tick", &GameComponentWrapper::on_tick)
		.def("on_input_event", &GameComponentWrapper::on_input_event)
		.def("on_input_event_base", &GameComponentWrapper::on_input_event_base)
		.def("on_render", &GameComponentWrapper::on_render)
        .add_property("owner", make_function(&GameComponentWrapper::get_owner, return_value_policy<copy_const_reference>()));

    class_<std::vector<boost::shared_ptr<game_object>>>("GameObjectVec")
        .def(vector_indexing_suite<std::vector<boost::shared_ptr<game_object>>>());

	class_<CameraParameters>("CameraParameters")
		.def_readonly("location", &CameraParameters::location)
		.def_readonly("projection_matrix", &CameraParameters::projection_matrix)
		.def_readonly("view_matrix", &CameraParameters::view_matrix);

	class_<Scene, boost::shared_ptr<Scene>, noncopyable>("Scene")
		.add_property("physics", make_function(&Scene::get_physics, return_value_policy<copy_const_reference>()))
		.def("create_game_object", &Scene::create_game_object)
		.def("remove_game_object", &Scene::remove_game_object)
		.def("render", &Scene::render)
		.def("tick", &Scene::tick)
		.def("on_input_event", &Scene::on_input_event)
		.def("trace", &Scene::trace)
        ;

    {
		auto camera_scope = class_<CameraComponent, bases<GameComponent>, boost::shared_ptr<CameraComponent>>(CameraComponent::component_name, init<>())
			.def_readwrite("near", &CameraComponent::near)
			.def_readwrite("far", &CameraComponent::far)
			.def_readwrite("fov", &CameraComponent::fov)
			.def_readwrite("projection_type", &CameraComponent::projection_type)
			.def("get_ray", &CameraComponent::get_ray)
            ;

        enum_<CameraComponent::ProjectionType>("ProjectionType")
			.value("ORTHOGRAPHIC", CameraComponent::ProjectionType::ORTHOGRAPHIC)
			.value("PERSPECTIVE", CameraComponent::ProjectionType::PERSPECTIVE)
            .export_values();
    }

	class_<Sound, bases<Resource>, boost::shared_ptr<Sound>, noncopyable>("Sound", no_init)
		.add_property("channel_count", &Sound::get_channel_count)
		.add_property("sample_rate", &Sound::get_sample_rate)
		.add_property("byte_rate", &Sound::get_byte_rate)
		.add_property("bits_per_sample", &Sound::get_bits_per_sample)
		.add_property("duration", &Sound::get_duration);

	class_<ModelComponent, bases<GameComponent>, boost::shared_ptr<ModelComponent>, noncopyable>("ModelComponent")
		.add_property("model", make_function(&ModelComponent::get_model, return_value_policy<copy_const_reference>()), &ModelComponent::set_model)
		.add_property("aabb", make_function(&ModelComponent::get_aabb, return_value_policy<return_by_value>()))
		.def("play", &ModelComponent::play);

	class_<ModelAnimation, boost::shared_ptr<ModelAnimation>, noncopyable>("ModelAnimation", no_init);

    // AUDIO
    class_<AudioSystem, noncopyable>("AudioSystem", no_init);

	class_<AudioSource, boost::shared_ptr<AudioSource>, noncopyable>("AudioSource", no_init);
        //.add_property("state", &audio_source::get_state);
        //.add_property("location", &audio_source::get_location)
        //;

    // PHYSICS
	class_<PhysicsSimulation, boost::shared_ptr<PhysicsSimulation>, noncopyable>("PhysicsSimulation")
		.add_property("gravity", &PhysicsSimulation::get_gravity, &PhysicsSimulation::set_gravity);

	class_<RigidBodyComponent, bases<GameComponent>, boost::shared_ptr<RigidBodyComponent>, noncopyable>(RigidBodyComponent::component_name)
		.add_property("mass", &RigidBodyComponent::get_mass, &RigidBodyComponent::set_mass)
		.add_property("center_of_mass", &RigidBodyComponent::get_center_of_mass, &RigidBodyComponent::set_center_of_mass)
		.add_property("aabb", &RigidBodyComponent::get_aabb);

    // TERRAIN
	class_<TerrainComponent, bases<GameComponent>, boost::shared_ptr<TerrainComponent>, noncopyable>(TerrainComponent::component_name)
		.add_property("scale", &TerrainComponent::get_scale, &TerrainComponent::set_scale)
		.def("trace", &TerrainComponent::trace)
		.def("set_heightmap", &TerrainComponent::set_heightmap);

	// PSK (TEMP)
	class_<PSK, boost::shared_ptr<PSK>, noncopyable>("PSK", init<const std::string&>())
		.def("render", &PSK::render);

    // HTTP
	enum_<HttpMethod>("HttpMethod")
		.value("GET", HttpMethod::GET)
		.value("POST", HttpMethod::POST)
		.value("PUT", HttpMethod::PUT)
		.value("DELETE", HttpMethod::DELETE_)
		.value("HEAD", HttpMethod::HEAD)
		.value("OPTIONS", HttpMethod::OPTIONS)
		.export_values();

	class_<HttpRequest, boost::shared_ptr<HttpRequest>, noncopyable>("HttpRequest", no_init)
		.add_property("url", make_function(&HttpRequest::get_url, return_value_policy<copy_const_reference>()))
		.add_property("method", &HttpRequest::get_method)
		.add_property("headers", make_function(&HttpRequest::get_headers, return_value_policy<copy_const_reference>()))
		;
	;

	class_<HttpResponse, boost::shared_ptr<HttpResponse>, noncopyable>("HttpResponse", no_init)
		.add_property("content", make_function(&HttpResponse::get_content, return_value_policy<copy_const_reference>()))
		.add_property("content_type", make_function(&HttpResponse::get_content_type, return_value_policy<copy_const_reference>()))
		.add_property("has_error", &HttpResponse::has_error)
		.add_property("elapsed", &HttpResponse::get_elapsed)
		;
	;

	// TODO: would be nice to just make a converter instead of having to give this a name
	// and legitimize it as an instantiable type.
	class_<std::vector<std::pair<std::string, std::string>>>("StringPairList")
		.def(vector_indexing_suite<std::vector<std::pair<std::string, std::string>>>());

    class_<HttpManager, noncopyable>("HttpManager", no_init)
		.def("get", &HttpManager::get,
		http_manager_get_overloads((
				boost::python::arg("url")
				//boost::python::arg("headers") = http_headers_type(),
				//boost::python::arg("data") = http_data_type(),
				//boost::python::arg("on_write") = make_function<void(size_t)>(nullptr)
			))
			)
        .def("get_async", &HttpManager::get_async, 
			http_manager_get_async_overloads(
				(
					boost::python::arg("url"),
					//boost::python::arg("headers") = http_headers_type(),
					//boost::python::arg("data") = http_data_type(),
					boost::python::arg("on_response") = boost::python::make_function<void(boost::shared_ptr<HttpResponse>)>(nullptr)
					//boost::python::arg("on_write") = make_function<void(size_t)>(nullptr)
				)
			)
		);

    scope().attr("http") = boost::ref(http);

	def("get_convex_hull", &Py_convex_hull);
}
