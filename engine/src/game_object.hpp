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
#include "type_object.hpp"
#include "hash.hpp"
#include "pose.hpp"

namespace naga
{
    struct game_component;
    struct camera_params;
    struct input_event_t;
    struct scene;

    struct game_object : boost::enable_shared_from_this<game_object>
    {
        typedef size_t id_type;

        pose3 pose;

        game_object();

        virtual void on_create() { }
        virtual void on_destroy() { }
        virtual bool on_input_event(input_event_t& input_event);
        virtual void on_tick(f32 dt);
        virtual void render(camera_params& camera);

        boost::python::object add_component_by_type(type_object type);
        boost::python::object add_component_by_name(const std::string& type);

        template<typename T = std::enable_if<std::is_base_of<game_component, T>::value>::type>
        boost::shared_ptr<T> add_component()
        {
            auto component_extract = boost::python::extract<boost::shared_ptr<T>>(add_component_by_name(T::component_name));

            if (!component_extract.check())
            {
				return nullptr;
            }

            return component_extract();
        }

        template<typename T = std::enable_if<std::is_base_of<game_component, T>::value>::type>
        boost::shared_ptr<T> get_component() const
        {
			auto component = get_component_by_name(T::component_name);

            return boost::static_pointer_cast<T, game_component>(component);
        }

        boost::shared_ptr<game_component> get_component_by_type(type_object type_object);
        boost::shared_ptr<game_component> get_component_by_name(const std::string& type) const;

        const boost::shared_ptr<scene>& get_scene() const { return scene; }
        id_type get_id() const { return id; }

    private:
        friend struct scene;

        id_type id;

        std::map<hash, std::vector<boost::shared_ptr<game_component>>> type_components;
        std::vector<boost::shared_ptr<game_component>> components;
        boost::shared_ptr<scene> scene;
    };
}
