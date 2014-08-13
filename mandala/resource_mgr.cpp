//mandala
#include "resource_mgr.hpp"

namespace mandala
{
	size_t resource_mgr_t::count() const
	{
		size_t count = 0;

		for (auto resources : type_resources)
		{
			count += resources.second.size();
		}

		return count;
	}

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
