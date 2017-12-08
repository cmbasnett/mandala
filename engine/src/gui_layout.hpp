#pragma once

//std
#include <typeindex>
#include <map>

//boost
#include <boost\shared_ptr.hpp>
#include <boost\weak_ptr.hpp>

//naga
#include "gui_node.hpp"
#include "input_event.hpp"

namespace naga
{
	struct GUILayout : GUINode
    {
        template<typename T = std::enable_if<std::is_base_of<GuiNode, T>::value>::type>
        void put(const std::string& name, boost::shared_ptr<T> node)
        {
            static const std::type_index TYPE_INDEX = typeid(T);

            auto type_nodes_itr = type_nodes.find(TYPE_INDEX);

            if (type_nodes_itr == type_nodes.end())
            {
				type_nodes.insert(type_nodes.begin(), std::make_pair(TYPE_INDEX, std::map<const std::string, boost::shared_ptr<GuiNode>>()));
            }

            auto& nodes = type_nodes.at(type_index);

            auto nodes_pair = nodes.insert(std::make_pair(name, node));

            if (!nodes_pair.second)
            {
                throw std::exception("node already exists for type");
            }
        }

        template<typename T = std::enable_if<std::is_base_of<GuiNode, T>::value>::type>
        boost::shared_ptr<T> get(const std::string& name)
        {
            static const std::type_index TYPE_INDEX = typeid(T);

            auto type_nodes_itr = type_nodes.find(TYPE_INDEX);

            if (type_nodes_itr == type_nodes.end())
            {
                throw std::out_of_range("node doesn't exist for type");
            }

            auto nodes_itr = type_nodes_itr->second.find(name);

            if (nodes_itr == type_nodes_itr->second.end())
            {
                throw std::out_of_range("node doesn't exist for type");
            }

            return boost::static_pointer_cast<T, gui_node>(nodes_itr->second);
        }

        template<typename T = std::enable_if<std::is_base_of<GuiNode, T>::value>::type>
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

        virtual bool on_input_event_begin(InputEvent& input_event) override;

    private:
		std::map<std::type_index, std::map<const std::string, boost::shared_ptr<GUINode>>> type_nodes;
		std::map<size_t, boost::weak_ptr<GUINode>> touch_nodes;
    };
}
