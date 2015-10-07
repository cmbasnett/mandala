#include "actor.hpp"
#include "bsp.hpp"
#include "model_instance.hpp"

namespace mandala
{
    void actor::on_tick(f32 dt)
    {
        if (model)
        {
            model->tick(dt);
        }
    }

    void actor::render(const camera_params& camera_params)
    {
        switch (draw_type)
        {
        case actor_draw_type::BSP:
            if (bsp)
            {
                bsp->render(camera_params);
            }
            break;
        case actor_draw_type::MODEL:
            if (model)
            {
                model->render(camera_params, vec3(100, 0, 0));
            }
            break;
        case actor_draw_type::NONE:
            break;
        }
    }
}