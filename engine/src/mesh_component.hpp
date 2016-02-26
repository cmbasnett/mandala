#pragma once

#include "game_component.hpp"
#include "model_component.hpp"

namespace naga
{
    struct mesh_component : game_component
    {
        boost::shared_ptr<model_component> model;
    };
}