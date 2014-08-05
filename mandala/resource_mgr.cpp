//mandala
#include "resource_mgr.hpp"

namespace mandala
{
	void resource_mgr_t::prune()
	{
		for (auto& type_resource : type_resources)
		{
			auto& resources = type_resource.second;

			auto resources_itr = resources.begin();

			while (resources_itr != resources.end())
			{
				if (resources_itr->second.use_count() == 1)
				{
					resources_itr = resources.erase(resources_itr);
				}
				else
				{
					++resources_itr;
				}
			}
		}
	}

	void resource_mgr_t::purge()
	{
		type_resources.clear();
	}
}
