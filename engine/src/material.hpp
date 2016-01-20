#pragma once

//naga
#include "resource.hpp"

//std
#include <memory>

namespace naga
{
    struct texture;
    struct gpu_program;

    struct material : resource
    {
        struct diffuse_t
        {
            boost::shared_ptr<texture> texture;
            vec4 color = vec4(1.0f);
        };

        struct normal_t
        {
            boost::shared_ptr<texture> texture;
        };

        struct specular_t
        {
            boost::shared_ptr<texture> texture;
            vec4 color = vec4(1);
            f32 intensity = 1.0f;
        };

        struct emissive_t
        {
            boost::shared_ptr<texture> texture;
            vec4 color = vec4(1);
            f32 intensity = 1.0f;
        };

        material(std::istream& istream);

        const bool get_is_two_sided() const { return is_two_sided; }
        const bool get_is_lit() const { return is_lit; }
        const diffuse_t& get_diffuse() const { return diffuse; }
        const normal_t& get_normal() const { return normal; }
        const specular_t& get_specular() const { return specular; }
        const emissive_t& get_emissive() const { return emissive; }

    private:
        bool is_two_sided = false;
        bool is_lit = true;
        diffuse_t diffuse;
        normal_t normal;
        specular_t specular;
        emissive_t emissive;

        material(const material&) = delete;
        material& operator=(const material&) = delete;

    };
}
