//mandala
#include "material_instance.hpp"

namespace mandala
{
    material_instance::material_instance(const boost::shared_ptr<material>& material)
    {
        *this = material;
    }

    material_instance& material_instance::operator=(const boost::shared_ptr<material>& material)
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

