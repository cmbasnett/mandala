#if defined(DEBUG)
//std
#include <vector>

//boost
#include <boost\weak_ptr.hpp>
#endif

//mandala
#include "resource_mgr.hpp"

namespace mandala
{
    resource_mgr_t resources;

    size_t resource_mgr_t::count() const
    {
        size_t count = 0;

        for (const auto& resources : type_resources)
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

    void resource_mgr_t::purge()
    {
#if defined(DEBUG)
        std::vector<boost::weak_ptr<resource_t>> resources;

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
        for (auto& resource : resources)
        {
            assert(resource.expired());
        }
#endif
    }
}
