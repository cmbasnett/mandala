#pragma once

//boost
#include <boost/shared_ptr.hpp>

//mandala
#include "pose.hpp"

namespace mandala
{
    struct model_instance_t;
    struct bsp_t;
    struct input_event_t;
    struct camera_params;

    struct actor_t
    {
        enum class draw_type_e
        {
            NONE,
            BSP,
            MODEL,
        };

        virtual void on_create() { }
        virtual void on_destroy() { }
        virtual bool on_input_event(input_event_t& input_event) { return false; }
        virtual void on_tick(float32_t dt);
        virtual void render(const camera_params& camera);

        pose3 pose;

        const boost::shared_ptr<model_instance_t>& get_model() const { return model; }
        void set_model(const boost::shared_ptr<model_instance_t>& model) { this->model = model; }

        const draw_type_e get_draw_type() const { return draw_type; }
        void set_draw_type(draw_type_e draw_type) { this->draw_type = draw_type; }

        const boost::shared_ptr<bsp_t>& get_bsp() const { return bsp; }
        void set_bsp(const boost::shared_ptr<bsp_t>& bsp) { this->bsp = bsp; }

    private:
        draw_type_e draw_type = draw_type_e::NONE;
        boost::shared_ptr<model_instance_t> model;
        boost::shared_ptr<bsp_t> bsp;
    };
}