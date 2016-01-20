#pragma once

// std
#include <vector>
#include <map>
#include <set>
#include <typeindex>

//boost
#include <boost\shared_ptr.hpp>
#include <boost/python.hpp>

// naga
#include "pose.hpp"
#include "hash.hpp"
#include "game_component.hpp"
#include "python.hpp"

namespace naga
{
    struct game_component;
    struct camera_params;
    struct input_event_t;

    struct game_object
    {
        pose3 pose;

        virtual void on_create() { }
        virtual void on_destroy() { }
        virtual bool on_input_event(input_event_t& input_event) { return false; }
        virtual void on_tick(f32 dt) { }
        virtual void render(const camera_params& camera) { }

        virtual void add_component(boost::python::object& type)
        {
            if (type.is_none())
            {
                throw std::invalid_argument("type cannot be None");
            }

            PyObject* object = type.ptr();

            auto game_object_type = py.eval("GameComponent");

            if (PyType_Check(object) == 0)
            {
                throw std::invalid_argument("argument \"type\" does not name a type");
            }

            if (PyObject_IsSubclass(type.ptr(), game_object_type.ptr()) == 0)
            {
                throw std::invalid_argument("type is not a subclass of GameComponent");
            }
        }

    private:
        std::map<std::type_index, boost::shared_ptr<game_component>> type_index_components;
        std::map<hash, boost::shared_ptr<game_component>> name_components;
    };
}