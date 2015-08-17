//mandala
#include "material_instance.hpp"

namespace mandala
{
    material_instance_t::material_instance_t(const boost::shared_ptr<material_t>& material)
    {
        *this = material;
    }

    material_instance_t& material_instance_t::operator=(const boost::shared_ptr<material_t>& material)
    {
        is_two_sided = material->get_is_two_sided();
        is_lit = material->get_is_lit();
        diffuse = material->get_diffuse();
        normal = material->get_normal();
        specular = material->get_specular();
        emissive = material->get_emissive();

        return *this;
    }
}

