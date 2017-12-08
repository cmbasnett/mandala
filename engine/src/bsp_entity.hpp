#pragma once

//std
#include <map>

//boost
#include <boost\optional.hpp>

namespace naga
{
    struct BSPEntity
    {
		BSPEntity(const std::string& string);

        const std::string& get_classname() const { return classname; }

        template<typename T = std::string>
        T get(const std::string& key) const
        {
            return boost::lexical_cast<T>(properties.at(key));
        }

        template<typename T = std::string>
        boost::optional<T> get_optional(const std::string& key) const
        {
            boost::optional<T> property = boost::none;

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
