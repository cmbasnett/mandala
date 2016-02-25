#pragma once

// boost
#include <boost/shared_ptr.hpp>

// naga
#include "types.hpp"
#include "game_component_defs.hpp"

namespace naga
{
    struct game_object;
    struct camera_params;
    struct input_event_t;

    struct game_component
    {
        const boost::shared_ptr<game_object>& get_owner() const { return owner; }

        virtual std::string get_component_name() const { return ""; }

        virtual void on_create() { }
        virtual void on_destroy() { }
        virtual void on_tick(f32 dt) { }
        virtual void on_render(camera_params& camera_params) { }
        virtual bool on_input_event(input_event_t& input_event) { return false; }

        virtual ~game_component() = default;

    protected:
        game_component() = default;

    private:
        friend struct game_object;
        
        boost::shared_ptr<game_object> owner;
    };
}