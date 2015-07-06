#pragma once

//boost
#include <boost/shared_ptr.hpp>

//mandala
#include "pose.hpp"

namespace mandala
{
    struct model_instance_t;

    struct actor_t
    {
        enum class draw_type_e
        {
            NONE,
            MODEL
        };

        virtual void on_create() { }
        virtual void on_destroy() { }
        virtual void on_tick(float32_t dt) { }

        pose3 pose;

        const boost::shared_ptr<model_instance_t>& get_model() const { return model; }
        const draw_type_e get_draw_type() const { return draw_type; }

        void set_model(const boost::shared_ptr<model_instance_t>& model) { this->model = model; }
        void set_draw_type(draw_type_e draw_type) { this->draw_type = draw_type; }

    private:
        draw_type_e draw_type = draw_type_e::NONE;
        boost::shared_ptr<model_instance_t> model;
    };
}