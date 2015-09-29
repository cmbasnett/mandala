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

    void actor_t::render(const boost::shared_ptr<camera_t>& camera)
    {
        switch (draw_type)
        {
        case draw_type_e::BSP:
            if (bsp)
            {
                bsp->render(camera);
            }
            break;
        case draw_type_e::MODEL:
            if (model)
            {
                model->render(camera, vec3_t());
            }
            break;
        case draw_type_e::NONE:
            break;
        }
    }
}