//#pragma once
//
////boost
//#include <boost/shared_ptr.hpp>
//
////naga
//#include "pose.hpp"
//
//namespace naga
//{
//    struct model_instance;
//    struct bsp;
//    struct input_event_t;
//    struct camera_params;
//
//    enum class actor_draw_type
//    {
//        NONE,
//        BSP,
//        MODEL,
//    };
//
//    struct actor
//    {
//        virtual void on_create() { }
//        virtual void on_destroy() { }
//        virtual bool on_input_event(input_event_t& input_event) { return false; }
//        virtual void on_tick(f32 dt);
//        virtual void render(const camera_params& camera);
//
//        pose3 pose;
//
//        const boost::shared_ptr<model_instance>& get_model() const { return model; }
//        void set_model(const boost::shared_ptr<model_instance>& model) { this->model = model; }
//
//        const actor_draw_type get_draw_type() const { return draw_type; }
//        void set_draw_type(actor_draw_type draw_type) { this->draw_type = draw_type; }
//
//        const boost::shared_ptr<bsp>& get_bsp() const { return bsp; }
//        void set_bsp(const boost::shared_ptr<bsp>& bsp) { this->bsp = bsp; }
//
//    private:
//        actor_draw_type draw_type = actor_draw_type::NONE;
//        boost::shared_ptr<model_instance> model;
//        boost::shared_ptr<bsp> bsp;
//    };
//}