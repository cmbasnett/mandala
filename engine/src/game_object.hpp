#pragma once

// std
#include <vector>
#include <map>
#include <set>

// boost
#include <boost\shared_ptr.hpp>
#include <boost\python.hpp>
#include <boost\enable_shared_from_this.hpp>

// naga
#include "pose.hpp"
#include "type_object.hpp"

namespace naga
{
    struct game_component;
    struct camera_params;
    struct input_event_t;

    struct game_object : boost::enable_shared_from_this<game_object>
    {
        pose3 pose;

        virtual void on_create() { }
        virtual void on_destroy() { }
        virtual bool on_input_event(input_event_t& input_event);
        virtual void on_tick(f32 dt);
        virtual void render(const camera_params& camera) { }

        void add_component_by_type(type_object type);
        void add_component_by_name(const std::string& type);

        template<typename T = std::enable_if<std::is_base_of<game_component, T>::value>::type>
        void add_component()
        {
            auto component = boost::make_shared<T>();

            components.insert(std::make_pair(component->get_type_name(), component));
        }

        template<typename T = std::enable_if<std::is_base_of<game_component, T>::value>::type>
        boost::shared_ptr<T> get_component() const
        {
            //TODO: component name is not guaranteed to be the same as the Python name.
            auto components_itr = components.find(T::component_name);

            if (components_itr == components.end())
            {
                return boost::shared_ptr<T>();
            }

            return boost::static_pointer_cast<T, game_component>(components_itr->second);
        }

        boost::shared_ptr<game_component> get_component_by_type(type_object type_object);
        boost::shared_ptr<game_component> get_component_by_name(const std::string& type) const;

    private:
        std::map<std::string, boost::shared_ptr<game_component>> components;
    };
}
