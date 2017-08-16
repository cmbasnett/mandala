#include "types.hpp"

namespace naga
{
	struct PSA
	{
		// Binary animation info format - used to organize raw animation keys into FAnimSeqs on rebuild
		// Similar to MotionChunkDigestInfo..
		struct animinfobinary
		{
			char name[64];
			char group[64];
			i32 total_bones;
			i32 root_include;	// 0 none 1 included
			i32 key_compression_style;
			i32 key_quotum;
			f32 key_reduction;
			f32 track_time;
			f32 fps;
			i32 start_bone;
			i32 first_raw_frame;
			i32 raw_frame_count;
		};

		struct QuatAnimKey
		{
			vec3 location;
			quat rotation;
			f32 time;
		};
	};
}