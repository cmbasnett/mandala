// naga
#include "game_object.hpp"
#include "python.hpp"
#include "game_component.hpp"
#include "camera_component.hpp"

namespace naga
{
	GameObject::GameObject()
    {
		static IdType next_id = 1;

		id = next_id++;
    }

	void GameObject::render(CameraParameters& camera_parameters)
    {
        for (auto& component : components)
        {
			component->on_render(camera_parameters);
        }
    }

	void GameObject::on_tick(f32 dt)
    {
        for (auto& component : components)
        {
            component->on_tick(dt);
        }
    }

	bool GameObject::on_input_event(InputEvent& input_event)
    {
        for (auto& component : components)
        {
            if (component->on_input_event(input_event))
            {
                return true;
            }
        }

        return false;
    }

	boost::python::object GameObject::add_component_by_type(type_object type)
    {
        static auto inspect = boost::python::import("inspect");

        boost::python::object component_object = type();

        // ensure type is convertible to GameComponent
		boost::python::extract<boost::shared_ptr<GameComponent>> component_extract(component_object);

        if (!component_extract.check())
        {
            throw std::exception("type is not convertible to GameComponent");
        }

        // method resolution order
        // we slice away the last 3 entries because they are inconsequential ([GameComponent, instance, object])
        // TODO: this may not always be the case, GameComponent might be elevated higher up a more complex hierarchy.
        //       it might be wise to determine the hierarchy (once) at runtime to see where GameComponent sits in
        //       the MRO list.
        auto mro = inspect.attr("getmro")(boost::python::object(type)).slice(0, -3);
        
        std::vector<std::string> base_names;

        for (auto i = 0; i < boost::python::len(mro); ++i)
        {
            auto base = mro[i];

			boost::python::extract<std::string> base_name(base.attr("__name__"));

            if (base_name.check())
            {
                base_names.push_back(base_name());
            }
        }

        auto component = component_extract();

        component->owner = shared_from_this();

        for (auto& base_name : base_names)
        {
            auto type_components_itr = type_components.find(base_name);

            if (type_components_itr == type_components.end())
            {
                type_components_itr = type_components.insert(type_components.begin(), std::make_pair(base_name, std::vector<boost::shared_ptr<GameComponent>>()));
            }

            type_components_itr->second.push_back(component);
        }

        components.push_back(component);

        component->on_create();

        return component_object;
    }

	boost::python::object GameObject::add_component_by_name(const std::string& name)
    {
		// TODO: naga.???
		auto type = py.eval(std::string("naga.").append(name).c_str());

        return add_component_by_type(type_object(type));
    }

	boost::shared_ptr<GameComponent> GameObject::get_component_by_type(type_object type)
    {
		boost::python::extract<std::string> name_extract(type.attr("__name__"));

        if (!name_extract.check())
        {
            throw std::invalid_argument("type is not convertible to GameComponent");
        }

        return get_component_by_name(name_extract());
    }

	boost::shared_ptr<GameComponent> GameObject::get_component_by_name(const std::string& type) const
    {
        auto type_components_itr = type_components.find(type);

        if (type_components_itr == type_components.end())
        {
            return boost::shared_ptr<GameComponent>();
        }

        // return the last component added
        return *type_components_itr->second.rbegin();
    }
}
