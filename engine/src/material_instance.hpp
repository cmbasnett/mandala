#pragma once

//boost
#include <boost\shared_ptr.hpp>

//naga
#include "material.hpp"

namespace naga
{
    struct material_instance
    {
        material_instance(const boost::shared_ptr<material>& material);

        bool get_is_two_sided() const { return is_two_sided; }
        bool get_is_lit() const { return is_lit; }
        material::diffuse_t& get_diffuse() { return diffuse; }
        material::normal_t& get_normal() { return normal; }
        material::specular_t& get_specular() { return specular; }
        material::emissive_t& get_emissive() { return emissive; }

        material_instance& operator=(const boost::shared_ptr<material>& material);

    private:
        bool is_two_sided = false;
        bool is_lit = true;
        material::diffuse_t diffuse;
        material::normal_t normal;
        material::specular_t specular;
        material::emissive_t emissive;
    };
}