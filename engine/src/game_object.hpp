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
#include "pose.hpp"

namespace naga
{
    struct GameComponent;
    struct CameraParameters;
	struct InputEvent;
    struct Scene;

	struct GameObject : boost::enable_shared_from_this<GameObject>
    {
        typedef size_t IdType;

		GameObject();

		Pose3 pose;

        virtual void on_create() { }
        virtual void on_destroy() { }
		virtual bool on_input_event(InputEvent& input_event);
        virtual void on_tick(f32 dt);
		virtual void render(CameraParameters& camera_parameters);

        boost::python::object add_component_by_type(type_object type);
        boost::python::object add_component_by_name(const std::string& type);

		template<typename T = std::enable_if<std::is_base_of<GameComponent, T>::value>::type>
        boost::shared_ptr<T> add_component()
        {
            auto component_extract = boost::python::extract<boost::shared_ptr<T>>(add_component_by_name(T::component_name));

            if (!component_extract.check())
            {
				return nullptr;
            }

            return component_extract();
        }

        template<typename T = std::enable_if<std::is_base_of<GameComponent, T>::value>::type>
        boost::shared_ptr<T> get_component() const
        {
			auto component = get_component_by_name(T::component_name);

			return boost::static_pointer_cast<T, GameComponent>(component);
        }

        boost::shared_ptr<GameComponent> get_component_by_type(type_object type_object);
		boost::shared_ptr<GameComponent> get_component_by_name(const std::string& type) const;

        const boost::shared_ptr<Scene>& get_scene() const { return scene; }
		IdType get_id() const { return id; }

    private:
        friend struct Scene;

		IdType id;

		std::map<std::string, std::vector<boost::shared_ptr<GameComponent>>> type_components;
		std::vector<boost::shared_ptr<GameComponent>> components;
		boost::shared_ptr<Scene> scene;
    };
}
