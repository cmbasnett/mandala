#pragma once

//boost
#include <boost\shared_ptr.hpp>

//naga
#include "material.hpp"

namespace naga
{
    struct MaterialInstance
    {
		MaterialInstance(const boost::shared_ptr<Material>& material);

        bool get_is_two_sided() const { return is_two_sided; }
        bool get_is_lit() const { return is_lit; }
		Material::Diffuse& get_diffuse() { return diffuse; }
		Material::Normal& get_normal() { return normal; }
		Material::Specular& get_specular() { return specular; }
        Material::Emissive& get_emissive() { return emissive; }

		MaterialInstance& operator=(const boost::shared_ptr<Material>& material);

    private:
        bool is_two_sided = false;
        bool is_lit = true;
		Material::Diffuse diffuse;
		Material::Normal normal;
		Material::Specular specular;
		Material::Emissive emissive;
    };
}