//boost
#include <boost\filesystem.hpp>

//lua
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "io.hpp"

//luabind
#include <luabind\luabind.hpp>
#include <luabind\operator.hpp>

//mandala
#include "lua_mgr.hpp"
#include "app.hpp"
#include "platform.hpp"

namespace mandala
{
    lua_mgr_t::lua_mgr_t()
    {
        state = luaL_newstate();

        luaopen_base(state);
        luaopen_string(state);
        //luaopen_table(state);
        //luaopen_math(state);
        //luaopen_io(state);
        //luaopen_debug(state);

        luabind::open(state);

        luabind::module(state)
            [
                luabind::class_<hash32_t>("hash")
                .def(luabind::constructor<>())
                .def(luabind::constructor<const hash32_t::string_type&>())
#ifdef _DEBUG
                .def("string", &hash32_t::string)
#endif
                .def("value", &hash32_t::value)
                .def(luabind::tostring(luabind::self))
                .def(luabind::self == hash32_t())
                .def(luabind::self <= hash32_t())
                .def(luabind::self < hash32_t())
            ];

        luabind::module(state)
            [
                luabind::class_<vec2_t>("vec2")
                .def(luabind::constructor<>())
                .def(luabind::constructor<float32_t>())
                .def(luabind::constructor<float32_t, float32_t>())
                .def_readwrite("x", &vec2_t::x)
                .def_readwrite("y", &vec2_t::y)
                .def_readwrite("r", &vec2_t::r)
                .def_readwrite("g", &vec2_t::g)
                .def_readwrite("s", &vec2_t::s)
                .def_readwrite("t", &vec2_t::t)
                .def(luabind::self + luabind::other<vec2_t>())
                .def(luabind::self - luabind::other<vec2_t>())
                .def(luabind::self == luabind::other<vec2_t>())
                .def(luabind::tostring(luabind::self))
            ];

        luabind::module(state)
            [
                luabind::class_<vec3_t>("vec3")
                .def(luabind::constructor<>())
                .def(luabind::constructor<float32_t>())
                .def(luabind::constructor<float32_t, float32_t, float32_t>())
                .def(luabind::constructor<vec2_t, float32_t>())
                .def_readwrite("x", &vec3_t::x)
                .def_readwrite("y", &vec3_t::y)
                .def_readwrite("z", &vec3_t::z)
                .def_readwrite("r", &vec3_t::r)
                .def_readwrite("g", &vec3_t::g)
                .def_readwrite("b", &vec3_t::b)
                .def_readwrite("s", &vec3_t::s)
                .def_readwrite("t", &vec3_t::t)
                .def_readwrite("p", &vec3_t::p)
                .def(luabind::tostring(luabind::self))
            ];

        luabind::module(state)
            [
                luabind::class_<vec4_t>("vec4")
                .def(luabind::constructor<>())
                .def(luabind::constructor<float32_t>())
                .def(luabind::constructor<float32_t, float32_t, float32_t, float32_t>())
                .def(luabind::constructor<vec3_t, float32_t>())
                .def_readwrite("x", &vec4_t::x)
                .def_readwrite("y", &vec4_t::y)
                .def_readwrite("z", &vec4_t::z)
                .def_readwrite("w", &vec4_t::w)
                .def_readwrite("r", &vec4_t::r)
                .def_readwrite("g", &vec4_t::g)
                .def_readwrite("b", &vec4_t::b)
                .def_readwrite("a", &vec4_t::a)
                .def_readwrite("s", &vec4_t::s)
                .def_readwrite("t", &vec4_t::t)
                .def_readwrite("p", &vec4_t::p)
                .def_readwrite("q", &vec4_t::q)
                .def(luabind::tostring(luabind::self))
            ];

        luabind::module(state)
            [
                luabind::class_<state_mgr_t>("state_mgr")
                .property("count", &state_mgr_t::count)
            ];

        luabind::module(state)
            [
                luabind::class_<pack_mgr_t>("pack_mgr")
            ];

        luabind::module(state)
            [
                luabind::class_<resource_mgr_t, luabind::bases<pack_mgr_t>>("resource_mgr")
                .def("prune", &resource_mgr_t::prune)
                .def("purge", &resource_mgr_t::purge)
            ];

        luabind::module(state)
            [
                luabind::class_<app_t>("app")
                .def("exit", &app_t::exit)
                .def("reset", &app_t::reset)
                .property("resources", &app_t::resources)
            ];

        luabind::globals(state)["app"] = &app;
    }

    lua_mgr_t::~lua_mgr_t()
    {
    }

    void lua_mgr_t::execute(const std::string& command)
    {
        if (luaL_dostring(state, command.c_str()))
        {
            const auto error_string = lua_tostring(state, -1);

            throw std::exception(error_string);
        }
    }
}
