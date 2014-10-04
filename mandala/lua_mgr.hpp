#pragma once

//std
#include <string>

struct lua_State;

namespace mandala
{
    struct lua_mgr_t
    {
        lua_mgr_t();
        ~lua_mgr_t();

        void execute(const std::string& command);

    private:
        lua_State* state;
    };
}