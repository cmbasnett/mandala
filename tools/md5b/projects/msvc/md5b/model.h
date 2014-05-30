#pragma once

//std
#include <string>
#include <vector>
#include <array>
#include <istream>

//glm
#include <glm\glm.hpp>
#include <glm\ext.hpp>

namespace md5b
{
    struct model_t
    {
        struct bone_t
        {
            std::string name;
            unsigned char parent_index;
            glm::vec3 position;
            glm::quat orientation;
        };

        struct mesh_t
        {
            struct vertex_t
            {
                glm::vec2 texcoord;
                unsigned short weight_index_start;
                unsigned char weight_count;
            };

            struct weight_t
            {
                unsigned char bone_index;
                float bias;
                glm::vec3 position;
            };

            std::string material;
            std::vector<vertex_t> vertices;
            std::vector<unsigned short> indices;
            std::vector<weight_t> weights;
        };

        std::vector<bone_t> bones;
        std::vector<mesh_t> meshes;

        model_t(std::istream& istream);

    private:
        model_t(const model_t&) = delete;
        model_t& operator=(const model_t&) = delete;
    };

    std::ostream& operator<<(std::ostream&, const model_t&);
};
