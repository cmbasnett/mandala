#pragma once

#include "types.hpp"

namespace naga
{
	struct TraceResult
	{
		bool did_hit = false;
		vec3 location;
		vec3 normal;
		void* optional = nullptr;
	};
}