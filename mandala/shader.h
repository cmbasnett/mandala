#pragma once

//mandala
#include "resource.h"

namespace mandala
{
	struct shader_t : resource_t
	{
		enum class type_t
		{
			vertex,
			fragment
		};

		shader_t(std::istream& istream);
		virtual ~shader_t();

		uint32_t handle = 0;
		type_t type = type_t::vertex;

	private:
		shader_t(shader_t&) = delete;
		shader_t& operator=(shader_t&) = delete;
	};
};
