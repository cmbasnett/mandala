#pragma once

#include "game_component.hpp"
#include "model_instance.hpp"

namespace naga
{
    struct mesh_component : game_component
    {
        boost::shared_ptr<model_instance> model;
    };
}