#pragma once

//std
#include <fstream>
#include <map>
#include <memory>
#include <mutex>
#include <typeindex>

//mandala
#include "hash.hpp"
#include "pack_mgr.hpp"
#include "resource.hpp"

namespace mandala
{
	struct resource_mgr_t : public pack_mgr_t
	{
		typedef std::map<hash_t, std::shared_ptr<resource_t>> resource_map_type;

		std::recursive_mutex mutex;

		size_t count() const;

		template<typename T = std::enable_if<is_resource<T>::value, T>::type>
		size_t count()
		{
			static const std::type_index type_index = typeid(T);

			const auto lock_guard = std::unique_lock<std::recursive_mutex>(mutex);

			const auto type_resources_itr = type_resources.find(type_index);

			if (type_resources_itr == type_resources.end())
			{
				return 0;
			}

			return type_resources_itr->second.size();
		}

		template<typename T = std::enable_if<is_resource<T>::value, T>::type>
		std::shared_ptr<T> get(const hash_t& hash)
		{
			static const std::type_index type_index = typeid(T);

			const auto lock_guard = std::unique_lock<std::recursive_mutex>(mutex);

			const auto type_resources_itr = type_resources.find(type_index);

			if (type_resources_itr == type_resources.end())
			{
				//no resources of this type have yet been allocated
                type_resources.insert(std::make_pair(type_index, resource_map_type()));
			}

			auto& resources = type_resources[type_index];

			auto resources_itr = resources.find(hash);

			if (resources_itr != resources.end())
			{
				//resource already exists
				auto& resource = resources_itr->second;

				resource->last_access_time = std::chrono::system_clock::now();

				return std::static_pointer_cast<T, resource_t>(resource);
			}

			auto istream = extract(hash);

			auto resource = std::make_shared<T>(*istream);

			resource->hash = hash;

			resources.emplace(hash, resource);

			return resource;
		}

		template<typename T = std::enable_if<is_resource<T>::value, T>::type>
		void put(std::shared_ptr<T> resource, const hash_t& hash)
		{
			static const std::type_index type_index = typeid(T);

			const auto lock_guard = std::unique_lock<std::recursive_mutex>(mutex);

			const auto type_resources_itr = type_resources.find(type_index);

			if (type_resources_itr == type_resources.end())
			{
				//no resources of this type allocated
                type_resources.insert(std::make_pair(type_index, resource_map_type()));
			}

			auto& resources = type_resources[type_index];

			const auto resources_itr = std::find_if(resources.begin(), resources.end(), [&](const std::pair<hash_t, std::shared_ptr<resource_t>>& pair)
			{
				return resource == pair.second;
			});

			if (resources_itr != resources.end())
			{
                std::ostringstream ostringstream;

                ostringstream << "resource " << hash << " already exists";

                throw std::exception(ostringstream.str().c_str());
			}

			resource->hash = hash;
			resource->last_access_time = std::chrono::system_clock::now();
			
			resources.emplace(resource->hash, resource);
		}

		void prune();
		void purge();

    private:
        std::map<std::type_index, resource_map_type> type_resources;
	};

	extern resource_mgr_t resources;
}
