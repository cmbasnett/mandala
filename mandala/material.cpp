//std
#include <istream>

//boost
#include <boost\property_tree\json_parser.hpp>

//glm
#include <glm\ext.hpp>

//mandala
#include "app.hpp"
#include "texture.hpp"
#include "material.hpp"
#include "gpu_program.hpp"
#include "boost.hpp"

namespace mandala
{
	material_t::material_t(std::istream& istream)
	{
		using namespace boost::property_tree;

		ptree ptree_;
		
		read_json(istream, ptree_);

		//gpu_program
		auto gpu_program_hash = ptree_.get<hash_t>("gpu_program");

		gpu_program = app.resources.get<gpu_program_t>(gpu_program_hash);

		if (gpu_program == nullptr)
		{
			throw std::exception();
		}

		//is_two_sided
		is_two_sided = ptree_.get<bool>("is_two_sided", false);

		//diffuse
		auto diffuse_optional = ptree_.get_child_optional("diffuse");

		if(diffuse_optional)
		{
			auto diffuse_ptree = diffuse_optional.get();

			//texture
			auto texture_hash = diffuse_ptree.get<hash_t>("texture");

			diffuse.texture = app.resources.get<texture_t>(texture_hash);

			//color
			auto color_optional = diffuse_ptree.get_child_optional("color");

			if(color_optional)
			{
				auto color_ptree = color_optional.get();
				auto color_value_ptr = glm::value_ptr(diffuse.color);

				//TODO: pointer operations are unsafe and obfuscate meaning, switch to something better
				if (color_ptree.size() != 4)
				{
					throw std::exception();
				}

				for(auto iterator = color_ptree.begin(); iterator != color_ptree.end(); ++iterator)
				{
					*(color_value_ptr++) = iterator->second.get_value<float32_t>();
				}
			}
		}

		//normal
		auto normal_optional = ptree_.get_child_optional("normal");

		if(normal_optional)
		{
			auto normal_ptree = normal_optional.get();

			//texture
			auto texture_hash = normal_ptree.get<hash_t>("texture");

			normal.texture = app.resources.get<texture_t>(texture_hash);
		}

		//specular
		auto specular_optional = ptree_.get_child_optional("specular");

		if(specular_optional)
		{
			auto specular_ptree = specular_optional.get();

			//texture
			auto texture_hash = specular_ptree.get<hash_t>("texture");

			specular.texture = app.resources.get<texture_t>(texture_hash);

			//color
			auto color_optional = specular_ptree.get_child_optional("color");

			if(color_optional)
			{
				auto color_ptree = color_optional.get();
				auto color_value_ptr = glm::value_ptr(specular.color);

				//TODO: pointer operations are unsafe and obfuscate meaning, switch to something better
				if (color_ptree.size() > 4)
				{
					throw std::exception();
				}

				for(auto iterator = color_ptree.begin(); iterator != color_ptree.end(); ++iterator)
				{
					*(color_value_ptr++) = iterator->second.get_value<float32_t>();
				}
			}

			//intensity
			auto intensity_optional = specular_ptree.get_optional<float32_t>("intensity");

			if(intensity_optional)
			{
				specular.intensity = intensity_optional.get();
			}
		}

		//emissive
		auto emissive_optional = ptree_.get_child_optional("emissive");

		if(emissive_optional)
		{
			auto emissive_ptree = emissive_optional.get();

			//texture
			auto texture_hash = emissive_ptree.get<hash_t>("texture");

			emissive.texture = app.resources.get<texture_t>(texture_hash);

			//color
			auto color_optional = emissive_ptree.get_child_optional("color");

			if(color_optional)
			{
				auto color_ptree = color_optional.get();
				auto color_value_ptr = glm::value_ptr(emissive.color);

				//TODO: pointer operations are unsafe and obfuscate meaning, switch to something better
				if (color_ptree.size() > 4)
				{
					throw std::exception();
				}

				for(auto iterator = color_ptree.begin(); iterator != color_ptree.end(); ++iterator)
				{
					*(color_value_ptr++) = iterator->second.get_value<float32_t>();
				}
			}

			//intensity
			auto intensity_optional = emissive_ptree.get_optional<float32_t>("intensity");

			if(intensity_optional)
			{
				emissive.intensity = intensity_optional.get();
			}
		}
	}
};
