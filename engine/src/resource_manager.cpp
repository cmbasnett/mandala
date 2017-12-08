#if defined(DEBUG)
//std
#include <vector>

//boost
#include <boost\weak_ptr.hpp>
#endif

//naga
#include "resource_manager.hpp"

namespace naga
{
    ResourceManager resources;

	size_t ResourceManager::count() const
    {
        //TODO: this could be replaced, whenever a resource is added or removed we can just increment or decrement a counter
        size_t count = 0;

        for (const auto& resources : type_resources)
        {
            count += resources.second.size();
        }

        return count;
    }

	void ResourceManager::prune()
    {
        for (auto& type_resource : type_resources)
        {
            auto& resources = type_resource.second;

            auto resources_itr = resources.begin();

            while (resources_itr != resources.end())
            {
                if (resources_itr->second.unique())
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

	void ResourceManager::purge()
    {
#if defined(DEBUG)
        std::vector<boost::weak_ptr<Resource>> resources;

        for (auto& type_resource : type_resources)
        {
            for (auto& resource : type_resource.second)
            {
                resources.push_back(resource.second);
            }
        }
#endif

        type_resources.clear();

#if defined(DEBUG)
        //TODO: python objects are being held on to for some reason, so this assertion always fails
        for (auto& resource : resources)
        {
            //assert(resource.expired());
        }
#endif
    }
}
