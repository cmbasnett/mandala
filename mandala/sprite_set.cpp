//std
#include <fstream>

//boost
#include <boost\property_tree\json_parser.hpp>

//mandala
#include "app.hpp"
#include "boost.hpp"
#include "sprite_set.hpp"
#include "texture.hpp"

namespace mandala
{
	sprite_set_t::sprite_set_t(std::istream& istream)
	{
		using namespace boost::property_tree;

		ptree ptree_;

		read_json(istream, ptree_);

		//meta
		auto meta_ptree = ptree_.get_child("meta");

		//texture
		auto texture_hash = meta_ptree.get<hash32_t>("image");

		texture = app.resources.get<texture_t>(texture_hash);

		//size
		auto size_ptree = meta_ptree.get_child("size");

		vec2_t texture_size;

		texture_size.x = size_ptree.get<vec2_t::value_type>("w");
		texture_size.y = size_ptree.get<vec2_t::value_type>("h");

		//sprites
		auto regions_ptree = ptree_.get_child("frames");

		for (auto& region_ptree : regions_ptree)
		{
			region_t region;

			//hash
			region.hash = region_ptree.second.get<hash_t>("filename");

			//frame_recangle
			auto frame_rectangle_ptree = region_ptree.second.get_child("frame");

			region.frame_rectangle.x = frame_rectangle_ptree.get<region_t::rectangle_type::scalar_type>("x");
			region.frame_rectangle.y = frame_rectangle_ptree.get<region_t::rectangle_type::scalar_type>("y");
			region.frame_rectangle.width = frame_rectangle_ptree.get<region_t::rectangle_type::scalar_type>("w");
			region.frame_rectangle.height = frame_rectangle_ptree.get<region_t::rectangle_type::scalar_type>("h");

			//is_rotated
			region.is_rotated = region_ptree.second.get<bool>("rotated");

			//is_trimmed
			region.is_trimmed = region_ptree.second.get<bool>("trimmed");

			//source_size
			auto source_size_ptree = region_ptree.second.get_child("sourceSize");

			region.source_size.x = source_size_ptree.get<region_t::size_type::value_type>("w");
			region.source_size.y = source_size_ptree.get<region_t::size_type::value_type>("h");

			//rectangle
			auto rectangle_ptree = region_ptree.second.get_child("spriteSourceSize");

			region.rectangle.x = rectangle_ptree.get<region_t::rectangle_type::scalar_type>("x");
			region.rectangle.y = region.source_size.y - rectangle_ptree.get<region_t::rectangle_type::scalar_type>("y") - region.frame_rectangle.height;
			region.rectangle.width = rectangle_ptree.get<region_t::rectangle_type::scalar_type>("w");
			region.rectangle.height = rectangle_ptree.get<region_t::rectangle_type::scalar_type>("h");

			if (region.is_rotated)
			{
				std::swap(region.frame_rectangle.width, region.frame_rectangle.height);
			}

			//uv
			region.uv.min.x = region.frame_rectangle.x / texture_size.x;
			region.uv.min.y = (texture_size.y - region.frame_rectangle.y - region.frame_rectangle.height) / texture_size.y;
			region.uv.max.x = (region.frame_rectangle.x + region.frame_rectangle.width) / texture_size.x;
			region.uv.max.y = (texture_size.y - region.frame_rectangle.y) / texture_size.y;

			region_hashes.push_back(region.hash);

			regions.insert(std::make_pair(region.hash, region));
		}
	}

	sprite_set_t::sprite_set_t(const std::shared_ptr<texture_t>& texture) :
		texture(texture)
	{
		region_t region;
		region.frame_rectangle.x = 0;
		region.frame_rectangle.y = 0;
		region.frame_rectangle.width = texture->width;
		region.frame_rectangle.height = texture->height;
		region.hash = texture->hash;
		region.is_rotated = false;
		region.is_trimmed = false;
		region.rectangle.x = 0;
		region.rectangle.y = 0;
		region.rectangle.width = texture->width;
		region.rectangle.height = texture->height;
		region.source_size.x = texture->width;
		region.source_size.y = texture->height;
		region.uv.min.x = 0.0f;
		region.uv.min.y = 0.0f;
		region.uv.max.x = 1.0f;
		region.uv.max.y = 1.0f;

		region_hashes.emplace_back();

		regions.emplace(hash_t(), region);
	}
}
