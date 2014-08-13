#pragma once

//mandala
#include "gui_node.hpp"
#include "hash.hpp"

//std
#include <typeindex>
#include <map>

namespace mandala
{
	struct gui_layout_t : gui_node_t
	{
		template<typename T = std::enable_if<std::is_base_of<gui_node_t, T>::value>::type>
		void put(const hash_t& hash, std::shared_ptr<T> node)
		{
			static const std::type_index type_index = typeid(T);

			auto type_nodes_itr = type_nodes.find(type_index);

			if (type_nodes_itr == type_nodes.end())
			{
				type_nodes.insert(type_nodes.begin(), std::make_pair(type_index, std::map<const hash_t, std::shared_ptr<gui_node_t>>()));
			}

			auto& nodes = type_nodes.at(type_index);

			auto nodes_pair = nodes.insert(std::make_pair(hash, node));

			if (!nodes_pair.second)
			{
				throw std::exception("node hash already exists for type");
			}
		}

		template<typename T = std::enable_if<std::is_base_of<gui_node_t, T>::value>::type>
		std::shared_ptr<T> get(const hash_t& hash)
		{
			static const std::type_index type_index = typeid(T);

			auto type_nodes_itr = type_nodes.find(type_index);

			if (type_nodes_itr == type_nodes.end())
			{
				throw std::out_of_range("node hash doesn't exist for type");
			}

			auto nodes_itr = type_nodes_itr->second.find(hash);

			if (nodes_itr == type_nodes_itr->second.end())
			{
				throw std::out_of_range("node hash doesn't exist for type");
			}

			return std::static_pointer_cast<T, gui_node_t>(nodes_itr->second);
		}

	private:
		std::map<std::type_index, std::map<const hash_t, std::shared_ptr<gui_node_t>>> type_nodes;
	};
}
