#pragma once

//mandala
#include "md5b.hpp"
#include "resource.hpp"
#include "model_skeleton.hpp"

namespace mandala
{
    struct model_animation_t : resource_t
    {
        struct bone_t
        {
            enum flags_t : u8
            {
                LOCATION_X    = (1 << 0),
                LOCATION_Y    = (1 << 1),
                LOCATION_Z    = (1 << 2),
                ROTATION_X    = (1 << 3),
                ROTATION_Y    = (1 << 4),
                ROTATION_Z    = (1 << 5),
            };
            
            hash_t hash;
            u8 parent_index = 0;
            u8 flags = 0;
            i32 data_start_index = 0;
        };
        
        model_animation_t(std::istream& istream);

        std::string name;
        u8 frames_per_second = 0;
        u16 frame_count = 0;
        u8 bone_count = 0;
        std::vector<model_skeleton_t> frame_skeletons;

    private:
        model_animation_t(const model_animation_t&) = delete;
        model_animation_t& operator=(const model_animation_t&) = delete;
    };
}
