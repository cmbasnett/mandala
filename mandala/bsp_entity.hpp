#pragma once

//std
#include <map>

//boost
#include <boost\optional.hpp>

namespace mandala
{
    struct bsp_entity_t
    {
        bsp_entity_t(const std::string& string);

        template<typename T = std::string>
        T get_property(const std::string& key) const
        {
            return boost::lexical_cast<T>(properties.at(key));
        }

        template<typename T = std::string>
        boost::optional<T> get_property_optional(const std::string& key) const
        {
            boost::optional<T> property;

            auto properties_itr = properties.find(key);

            if (properties_itr != properties.end())
            {
                try
                {
                    property = boost::lexical_cast<T>(properties_itr->second);
                }
                catch (boost::bad_lexical_cast e)
                {
                }
            }

            return property;
        }

    private:
        std::map<std::string, std::string> properties;
		std::string classname;
    };
}
