#pragma once

//std
#include <memory>

//mandala
#include "mandala.hpp"
#include "resource.hpp"

namespace mandala
{
	struct image_t;

	struct texture_t : resource_t
	{
		int32_t width = 0;
		int32_t height = 0;
		uint32_t handle = 0;
		uint32_t internal_format = 0;
		uint32_t format = 0;
		bool has_alpha = false;

		texture_t(std::shared_ptr<image_t> image);
		texture_t(std::istream& istream);
		virtual ~texture_t();

	private:
		texture_t(texture_t&) = delete;
		texture_t& operator=(texture_t&) = delete;
	};
};
