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

		const std::string& classname() const { return _classname; }

        template<typename T = std::string>
        T get(const std::string& key) const
        {
			return boost::lexical_cast<T>(_properties.at(key));
        }

        template<typename T = std::string>
        boost::optional<T> get_optional(const std::string& key) const
        {
			boost::optional<T> property = boost::none;

            auto properties_itr = _properties.find(key);

			if (properties_itr != _properties.end())
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
        std::map<std::string, std::string> _properties;
		std::string _classname;
    };
}
