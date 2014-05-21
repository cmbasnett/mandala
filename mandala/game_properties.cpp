//mandala
#include "game_properties.h"

//boost
#include "boost\property_tree\json_parser.hpp"

//glm
#include "glm\ext.hpp"

namespace mandala
{
	using namespace boost::property_tree;

	game_properties_t::game_properties_t()
	{
	}

	game_properties_t::game_properties_t(std::istream& istream)
	{
		ptree ptree;

		read_json(istream, ptree);

		//name
		name = ptree.get<std::string>("name");

		//resources.packs
		auto resources_packs_ptree_optional = ptree.get_child_optional("resources.packs");

		if (resources_packs_ptree_optional)
		{
			auto resources_packs_ptree = resources_packs_ptree_optional.get();

			for (auto resources_pack_ptree_pair : resources_packs_ptree)
			{
				resources.packs.push_back(resources_pack_ptree_pair.second.get_value<std::string>());
			}
		}

		//window.size
		auto window_size_ptree_optional = ptree.get_child_optional("window.size");

		if (window_size_ptree_optional)
		{
			auto window_size_ptree = window_size_ptree_optional.get();
			auto window_size_value_ptr = glm::value_ptr(window.size);

			if (window_size_ptree.size() != 2)
			{
				throw std::exception();
			}
			
			for (auto window_size_ptree_itr = window_size_ptree.begin(); window_size_ptree_itr != window_size_ptree.end(); ++window_size_ptree_itr)
			{
				*(window_size_value_ptr++) = window_size_ptree_itr->second.get_value<window_t::size_type_t::size_type>();
			}
		}

		//window.position
		auto window_position_ptree_optional = ptree.get_child_optional("window.position");

		if (window_position_ptree_optional)
		{
			auto window_position_ptree = window_position_ptree_optional.get();
			auto window_position_value_ptr = glm::value_ptr(window.position);

			if (window_position_ptree.size() != 2)
			{
				throw std::exception();
			}

			for (auto window_position_ptree_itr = window_position_ptree.begin(); window_position_ptree_itr != window_position_ptree.end(); ++window_position_ptree_itr)
			{
				*(window_position_value_ptr++) = window_position_ptree_itr->second.get_value<window_t::position_type_t::size_type>();
			}
		}
	}
};
