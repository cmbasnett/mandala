//std
#include <iostream>

//boost
#include <boost\property_tree\json_parser.hpp>

//glm
#include <glm\ext.hpp>

//naga
#include "resource_manager.hpp"
#include "texture.hpp"
#include "material.hpp"
#include "boost.hpp"

namespace naga
{
	Material::Material(std::istream& istream)
    {
        using namespace boost::property_tree;

        ptree ptree_;
        
        try
        {
            read_json(istream, ptree_);
        }
        catch (json_parser_error e)
        {
            std::cout << e.what() << std::endl;
        }

        //is two sided
        is_two_sided = ptree_.get<bool>("is_two_sided", is_two_sided);

        //is lit
        is_lit = ptree_.get<bool>("is_lit", is_lit);

        //diffuse
        const auto diffuse_optional = ptree_.get_child_optional("diffuse");

        if (diffuse_optional)
        {
            const auto diffuse_ptree = diffuse_optional.get();

            //texture
			diffuse.texture = resources.get<Texture>(diffuse_ptree.get<std::string>("texture"));

            //color
            const auto color_optional = diffuse_ptree.get_child_optional("color");

            if(color_optional)
            {
                const auto color_ptree = color_optional.get();
                auto color_value_ptr = glm::value_ptr(diffuse.color);

                //TODO: pointer operations are unsafe and obfuscate meaning, switch to something better
                if (color_ptree.size() != 4)
                {
                    throw std::exception();
                }

                for(auto iterator = color_ptree.begin(); iterator != color_ptree.end(); ++iterator)
                {
                    *(color_value_ptr++) = iterator->second.get_value<f32>();
                }
            }
        }

        //normal
        const auto normal_optional = ptree_.get_child_optional("normal");

        if(normal_optional)
        {
            const auto normal_ptree = normal_optional.get();

            //texture
			normal.texture = resources.get<Texture>(normal_ptree.get<std::string>("texture"));
        }

        //specular
        auto specular_optional = ptree_.get_child_optional("specular");

        if(specular_optional)
        {
            const auto specular_ptree = specular_optional.get();

            //texture
			specular.texture = resources.get<Texture>(specular_ptree.get<std::string>("texture"));

            //color
            const auto color_optional = specular_ptree.get_child_optional("color");

            if(color_optional)
            {
                const auto color_ptree = color_optional.get();
                auto color_value_ptr = glm::value_ptr(specular.color);

                //TODO: pointer operations are unsafe and obfuscate meaning, switch to something better
                if (color_ptree.size() > 4)
                {
                    throw std::exception();
                }

                for(auto iterator = color_ptree.begin(); iterator != color_ptree.end(); ++iterator)
                {
                    *(color_value_ptr++) = iterator->second.get_value<f32>();
                }
            }

            //intensity
            const auto intensity_optional = specular_ptree.get_optional<f32>("intensity");

            if(intensity_optional)
            {
                specular.intensity = intensity_optional.get();
            }
        }

        //emissive
        auto emissive_optional = ptree_.get_child_optional("emissive");

        if(emissive_optional)
        {
            const auto emissive_ptree = emissive_optional.get();

            //texture
			emissive.texture = resources.get<Texture>(emissive_ptree.get<std::string>("texture"));

            //color
            const auto color_optional = emissive_ptree.get_child_optional("color");

            if(color_optional)
            {
                const auto color_ptree = color_optional.get();
                auto color_value_ptr = glm::value_ptr(emissive.color);

                //TODO: pointer operations are unsafe and obfuscate meaning, switch to something better
                if (color_ptree.size() > 4)
                {
                    throw std::exception();
                }

                for(auto iterator = color_ptree.begin(); iterator != color_ptree.end(); ++iterator)
                {
                    *(color_value_ptr++) = iterator->second.get_value<f32>();
                }
            }

            //intensity
            const auto intensity_optional = emissive_ptree.get_optional<f32>("intensity");

            if(intensity_optional)
            {
                emissive.intensity = intensity_optional.get();
            }
        }
    }
}
