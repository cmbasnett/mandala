// naga
#include "game_object.hpp"
#include "python.hpp"
#include "game_component.hpp"

namespace naga
{
    void game_object::on_tick(f32 dt)
    {
        for (auto& component : components)
        {
            component.second->on_tick(dt);
        }
    }

    bool game_object::on_input_event(input_event_t& input_event)
    {
        for (auto& component : components)
        {
            if (component.second->on_input_event(input_event))
            {
                return true;
            }
        }

        return false;
    }

    void game_object::add_component_by_type(type_object type)
    {
        const boost::python::object component_object = type();

        auto component_extract = boost::python::extract<boost::shared_ptr<game_component>>(component_object.ptr());

        if (!component_extract.check())
        {
            throw std::exception("type is not convertible to GameComponent");
        }

        auto name = boost::python::extract<std::string>(type.attr("__name__"))();
        auto component = component_extract();

        component->owner = shared_from_this();

        components.insert(std::make_pair(name, component));
    }

    void game_object::add_component_by_name(const std::string& name)
    {
        auto type = py.eval(name.c_str());

        add_component_by_type(type_object(type));
    }

    boost::shared_ptr<game_component> game_object::get_component_by_type(type_object type)
    {
        auto name_extract = boost::python::extract<std::string>(type.attr("__name__"));

        if (!name_extract.check())
        {
            throw std::invalid_argument("type is not convertible to GameComponent");
        }

        return get_component_by_name(name_extract());
    }

    boost::shared_ptr<game_component> game_object::get_component_by_name(const std::string& type) const
    {
        auto components_itr = components.find(type);

        if (components_itr == components.end())
        {
            return boost::shared_ptr<game_component>();
        }

        return components_itr->second;
    }
}
