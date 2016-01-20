#pragma once

//naga
#include "md5b.hpp"
#include "resource.hpp"
#include "model_skeleton.hpp"

namespace naga
{
    struct model_animation : resource
    {
        struct bone
        {
            enum flags : u8
            {
                LOCATION_X    = (1 << 0),
                LOCATION_Y    = (1 << 1),
                LOCATION_Z    = (1 << 2),
                ROTATION_X    = (1 << 3),
                ROTATION_Y    = (1 << 4),
                ROTATION_Z    = (1 << 5),
            };
            
            naga::hash hash;
            u8 parent_index = 0;
            u8 flags = 0;
            i32 data_start_index = 0;
        };
        
        model_animation(std::istream& istream);

        std::string name;
        u8 frames_per_second = 0;
        u16 frame_count = 0;
        u8 bone_count = 0;
        std::vector<model_skeleton> frame_skeletons;

    private:
        model_animation(const model_animation&) = delete;
        model_animation& operator=(const model_animation&) = delete;
    };
}
