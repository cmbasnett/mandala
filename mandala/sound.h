#pragma once

//std
#include <vector>

//mandala
#include "resource.h"

namespace mandala
{
	struct sound_t : resource_t
	{
		typedef std::chrono::milliseconds duration_type_t;

		sound_t(std::istream& istream);
		virtual ~sound_t();

		int16_t channel_count = 0;
		int32_t sample_rate = 0;
		int32_t byte_rate = 0;
		int16_t block_align = 0;
		int16_t bits_per_sample = 0;
		uint32_t buffer = 0;
		duration_type_t duration;
	};
};
