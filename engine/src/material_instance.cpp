//naga
#include "material_instance.hpp"

namespace naga
{
	MaterialInstance::MaterialInstance(const boost::shared_ptr<Material>& material)
    {
        *this = material;
    }

	MaterialInstance& MaterialInstance::operator=(const boost::shared_ptr<Material>& material)
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

