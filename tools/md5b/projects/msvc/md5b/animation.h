#pragma once

//std
#include <vector>
#include <string>

//glm
#include <glm\glm.hpp>
#include <glm\ext.hpp>

namespace md5b
{
    struct animation_t
    {
        struct bone_t
        {
            enum class flags_t : unsigned char
            {
                position_x = (1 << 0),
                position_y = (1 << 1),
                position_z = (1 << 2),
                orientation_x = (1 << 3),
                orientation_y = (1 << 4),
                orientation_z = (1 << 5)
            };

            std::string name;
            unsigned char parent_index = 0;
            std::underlying_type<flags_t>::type flags;
            int frame_data_start_index = 0;
        };

        struct bone_frame_t
        {
            glm::vec3 position;
            glm::quat orientation;
        };

        struct frame_t
        {
            glm::vec3 min;
            glm::vec3 max;
            std::vector<float> data;
        };

        animation_t(std::istream& istream);

        std::vector<bone_frame_t> base_bone_frames;
        std::vector<frame_t> frames;
        std::vector<bone_t> bones;
        unsigned char frames_per_second = 0;
        int frame_data_component_count = 0;
    };

    std::ostream& operator<<(std::ostream&, const animation_t&);
};
