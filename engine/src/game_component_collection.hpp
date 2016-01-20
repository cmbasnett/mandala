#pragma once

// std
#include <vector>
#include <map>
#include <set>
#include <typeindex>

//boost
#include <boost\shared_ptr.hpp>

//naga
#include "hash.hpp"
#include "game_component.hpp"

namespace naga
{
    struct game_component_collection
    {
        template<typename T = std::is_base_of<game_component, T>::type>
        void add(const boost::shared_ptr<T>& game_component)
        {
            static std::type_index type_index = typeid(T);

            auto itr = type_index_components.find(type_index);

            if (itr == type_index_components.end())
            {

            }
        }

    private:
        std::map<std::type_index, boost::shared_ptr<game_component>> type_index_components;
        std::map<hash, boost::shared_ptr<game_component>> name_components;
    };
}