#include "actor.hpp"
#include "bsp.hpp"
#include "model_instance.hpp"

namespace mandala
{
    void actor_t::on_tick(float32_t dt)
    {
        if (model)
        {
            model->tick(dt);
        }
    }

    void actor_t::render(const camera_params& camera_params)
    {
        switch (draw_type)
        {
        case draw_type_e::BSP:
            if (bsp)
            {
                bsp->render(camera_params);
            }
            break;
        case draw_type_e::MODEL:
            if (model)
            {
                model->render(camera_params, vec3_t(100, 0, 0));
            }
            break;
        case draw_type_e::NONE:
            break;
        }
    }
}