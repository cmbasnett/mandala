#pragma once

//std
#include <fstream>
#include <map>
#include <mutex>
#include <typeindex>
#include <sstream>

//boost
#include <boost\shared_ptr.hpp>
#include <boost\make_shared.hpp>
#include <boost\uuid\random_generator.hpp>
#include <boost\uuid\uuid_io.hpp>

//naga
#include "package_manager.hpp"
#include "resource.hpp"

namespace naga
{
    struct ResourceManager : public PackageManager
    {
        typedef std::map<std::string, boost::shared_ptr<Resource>> resource_map_type;

        size_t count() const;

        template<typename T = std::enable_if<IsResource<T>::value, T>::type>
        size_t count()
        {
            static const std::type_index TYPE_INDEX = typeid(T);

            std::lock_guard<std::recursive_mutex> lock(mutex);

            const auto type_resources_itr = type_resources.find(TYPE_INDEX);

            if (type_resources_itr == type_resources.end())
            {
                return 0;
            }

            return type_resources_itr->second.size();
        }

        template<typename T = std::enable_if<IsResource<T>::value, T>::type>
        boost::shared_ptr<T> get(const std::string& name)
        {
            static const std::type_index TYPE_INDEX = typeid(T);

            std::lock_guard<std::recursive_mutex> lock(mutex);

            const auto type_resources_itr = type_resources.find(TYPE_INDEX);

			if (type_resources_itr == type_resources.end())
			{
				//no resources of this type have yet been allocated
				type_resources.insert(std::make_pair(TYPE_INDEX, resource_map_type()));
			}

            auto& resources = type_resources[TYPE_INDEX];
			auto resources_itr = resources.find(name);

            if (resources_itr != resources.end())
            {
                //resource already exists
                auto& resource = resources_itr->second;

                resource->last_access_time = Resource::ClockType::now();

                return boost::static_pointer_cast<T, Resource>(resource);
            }

			boost::shared_ptr<std::istream> istream;

			try
			{
				istream = extract(name);
			}
			catch (const std::out_of_range&)
			{
				// TODO: not in the packs, check the file system!
				istream = boost::make_shared<std::ifstream>(name, std::ios::binary);
			}

            auto resource = boost::make_shared<T>(*istream);
            resource->name = name;

            resources.emplace(name, resource);

            return resource;
		}
		
		template<typename T, typename... Args>
		boost::shared_ptr<T> make(Args&&... args)
		{
			auto resource = boost::make_shared<T>(args...);
			put(resource);
			return resource;
		}

        template<typename T = std::enable_if<IsResource<T>::value, T>::type>
        void put(boost::shared_ptr<T> resource)
        {
            static const std::type_index TYPE_INDEX = typeid(T);

            std::lock_guard<std::recursive_mutex> lock(mutex);

            const auto type_resources_itr = type_resources.find(TYPE_INDEX);

            if (type_resources_itr == type_resources.end())
            {
                //no resources of this type allocated
                type_resources.insert(std::make_pair(TYPE_INDEX, resource_map_type()));
            }

            auto& resources = type_resources[TYPE_INDEX];

            const auto resources_itr = std::find_if(resources.begin(), resources.end(), [&](const std::pair<std::string, boost::shared_ptr<Resource>>& pair)
            {
                return resource == pair.second;
            });

            if (resources_itr != resources.end())
            {
                std::ostringstream oss;

				oss << "resource " << resources_itr->first << " already exists";

				throw std::exception(oss.str().c_str());
            }

			// generate a globally unique identifier for this new resource.
			static boost::uuids::random_generator random_generator;

			resource->name = boost::uuids::to_string(random_generator());
            resource->last_access_time = std::chrono::system_clock::now();
            
            resources.emplace(resource->name, resource);
        }

        void prune();
        void purge();

    private:
        std::recursive_mutex mutex;
        std::map<std::type_index, resource_map_type> type_resources;
    };

    extern ResourceManager resources;
}
