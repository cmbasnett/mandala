#pragma once

//naga
#include "md5b.hpp"
#include "resource.hpp"
#include "model_skeleton.hpp"

namespace naga
{
	struct ModelAnimation : Resource
    {
        struct Bone
        {
            enum Flags : u8
            {
                LOCATION_X    = (1 << 0),
                LOCATION_Y    = (1 << 1),
                LOCATION_Z    = (1 << 2),
                ROTATION_X    = (1 << 3),
                ROTATION_Y    = (1 << 4),
                ROTATION_Z    = (1 << 5),
            };
            
			std::string name;
            u8 parent_index = 0;
            u8 flags = 0;
            i32 data_start_index = 0;
        };
        
		ModelAnimation(std::istream& istream);

        std::string name;
        u8 frames_per_second = 0;
        u16 frame_count = 0;
        u8 bone_count = 0;
        std::vector<ModelSkeleton> frame_skeletons;

    private:
		ModelAnimation(const ModelAnimation&) = delete;
		ModelAnimation& operator=(const ModelAnimation&) = delete;
    };
}
