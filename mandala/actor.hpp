#pragma once

//std
#include <memory>
#include <vector>

//mandala
#include "types.hpp"

namespace mandala
{
    struct model_instance_t;

    struct actor_t : std::enable_shared_from_this<actor_t>
    {
        enum class movement_type_e
        {
            independent,
        };

        enum class rotation_type_e
        {
            none,
        };

        enum class render_type_e
        {
            none,
            model
        };

        vec3_t position;
        quat_t rotation;
        vec3_t scale = vec3_t(1);

        void orphan();
        void adopt(std::shared_ptr<actor_t> actor);

        movement_type_e movement_type;
        rotation_type_e rotation_type;
        render_type_e render_type;

        std::shared_ptr<model_instance_t> model;

        virtual void tick(float32_t elapsed_seconds);
        virtual void render();

    private:
        std::vector<std::shared_ptr<actor_t>> children;
    };
}