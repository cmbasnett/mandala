#pragma once

// boost
#include <boost/shared_ptr.hpp>

// naga
#include "types.hpp"
#include "game_component_defs.hpp"

namespace naga
{
    struct GameObject;
	struct CameraParameters;
    struct InputEvent;

    struct GameComponent
    {
		const boost::shared_ptr<GameObject>& get_owner() const { return owner; }

        virtual std::string get_component_name() const { return ""; }

        virtual void on_create() { }
        virtual void on_destroy() { }
        virtual void on_tick(f32 dt) { }
		virtual void on_render(CameraParameters& camera_parameters) { }
		virtual bool on_input_event(InputEvent& input_event) { return false; }

		virtual ~GameComponent() = default;

    protected:
		GameComponent() = default;

    private:
        friend struct GameObject;
        
		boost::shared_ptr<GameObject> owner;
    };
}