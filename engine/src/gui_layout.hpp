#pragma once

//std
#include <typeindex>
#include <map>

//boost
#include <boost\shared_ptr.hpp>
#include <boost\weak_ptr.hpp>

//naga
#include "gui_node.hpp"
#include "hash.hpp"
#include "input_event.hpp"

namespace naga
{
    struct gui_layout : gui_node
    {
        template<typename T = std::enable_if<std::is_base_of<gui_node, T>::value>::type>
        void put(const hash& hash, boost::shared_ptr<T> node)
        {
            static const std::type_index TYPE_INDEX = typeid(T);

            auto type_nodes_itr = type_nodes.find(TYPE_INDEX);

            if (type_nodes_itr == type_nodes.end())
            {
                type_nodes.insert(type_nodes.begin(), std::make_pair(type_index, std::map<const hash, boost::shared_ptr<gui_node>>()));
            }

            auto& nodes = type_nodes.at(type_index);

            auto nodes_pair = nodes.insert(std::make_pair(hash, node));

            if (!nodes_pair.second)
            {
                throw std::exception("node hashalready exists for type");
            }
        }

        template<typename T = std::enable_if<std::is_base_of<gui_node, T>::value>::type>
        boost::shared_ptr<T> get(const hash& hash)
        {
            static const std::type_index TYPE_INDEX = typeid(T);

            auto type_nodes_itr = type_nodes.find(TYPE_INDEX);

            if (type_nodes_itr == type_nodes.end())
            {
                throw std::out_of_range("node hashdoesn't exist for type");
            }

            auto nodes_itr = type_nodes_itr->second.find(hash);

            if (nodes_itr == type_nodes_itr->second.end())
            {
                throw std::out_of_range("node hashdoesn't exist for type");
            }

            return boost::static_pointer_cast<T, gui_node>(nodes_itr->second);
        }

        template<typename T = std::enable_if<std::is_base_of<gui_node, T>::value>::type>
        size_t count()
        {
            static const std::type_index TYPE_INDEX = typeid(T);

            auto type_nodes_itr = type_nodes.find(TYPE_INDEX);

            if (type_nodes_itr == type_nodes.end())
            {
                return 0;
            }

            return type_nodes_itr->second.size();
        }

        virtual bool on_input_event_begin(input_event_t& input_event) override;

    private:
        std::map<std::type_index, std::map<const hash, boost::shared_ptr<gui_node>>> type_nodes;
        std::map<input_event_t::touch_t::touch_id_type, boost::weak_ptr<gui_node>> touch_nodes;
    };
}
