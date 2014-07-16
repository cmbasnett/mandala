#pragma once

//mandala
#include "resource.hpp"

//std
#include <memory>

namespace mandala
{
	struct texture_t;
	struct gpu_program_t;

	struct material_t : resource_t
	{
		struct diffuse_t
		{
			std::shared_ptr<texture_t> texture;
			vec4_t color = vec4_t(1.0f);
		};

		struct normal_t
		{
			std::shared_ptr<texture_t> texture;
		};

		struct specular_t
		{
			std::shared_ptr<texture_t> texture;
			vec4_t color = vec4_t(1.0f);
			float32_t intensity = 1.0f;
		};

		struct emissive_t
		{
			std::shared_ptr<texture_t> texture;
			vec4_t color = vec4_t(1.0f);
			float32_t intensity = 1.0f;
		};

		material_t(std::istream& ifstream);

		std::shared_ptr<gpu_program_t> gpu_program;
		bool is_two_sided = true;
		diffuse_t diffuse;
		normal_t normal;
		specular_t specular;
		emissive_t emissive;

	private:
		material_t(const material_t&) = delete;
		material_t& operator=(const material_t&) = delete;
	};
};
