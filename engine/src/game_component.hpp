#pragma once

// boost
#include <boost/shared_ptr.hpp>

namespace naga
{
    struct game_object;

    struct game_component
    {
        boost::shared_ptr<game_object> get_owner() const { return owner; }

        virtual void on_tick(f32 dt) { }

        virtual ~game_component() = default;

    protected:
        game_component() = default;

    private:
        boost::shared_ptr<game_object> owner;
    };
}