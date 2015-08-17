#pragma once

//boost
#include <boost\shared_ptr.hpp>

//mandala
#include "material.hpp"

namespace mandala
{
    struct material_instance_t
    {
        material_instance_t(const boost::shared_ptr<material_t>& material);

        bool get_is_two_sided() const { return is_two_sided; }
        bool get_is_lit() const { return is_lit; }
        material_t::diffuse_t& get_diffuse() { return diffuse; }
        material_t::normal_t& get_normal() { return normal; }
        material_t::specular_t& get_specular() { return specular; }
        material_t::emissive_t& get_emissive() { return emissive; }

        material_instance_t& operator=(const boost::shared_ptr<material_t>& material);

    private:
        bool is_two_sided = false;
        bool is_lit = true;
        material_t::diffuse_t diffuse;
        material_t::normal_t normal;
        material_t::specular_t specular;
        material_t::emissive_t emissive;
    };
}