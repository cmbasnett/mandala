#pragma once

//naga
#include "resource.hpp"
#include "types.hpp"

//std
#include <memory>

namespace naga
{
    struct Texture;

	struct Material : Resource
    {
        struct Diffuse
        {
			boost::shared_ptr<Texture> texture;
            vec4 color = vec4(1.0f);
        };

        struct Normal
        {
			boost::shared_ptr<Texture> texture;
        };

        struct Specular
        {
			boost::shared_ptr<Texture> texture;
            vec4 color = vec4(1);
            f32 intensity = 1.0f;
        };

        struct Emissive
        {
			boost::shared_ptr<Texture> texture;
            vec4 color = vec4(1);
            f32 intensity = 1.0f;
        };

        Material(std::istream& istream);

        const bool get_is_two_sided() const { return is_two_sided; }
        const bool get_is_lit() const { return is_lit; }
        const Diffuse& get_diffuse() const { return diffuse; }
        const Normal& get_normal() const { return normal; }
		const Specular &get_specular() const { return specular; }
        const Emissive& get_emissive() const { return emissive; }

    private:
        bool is_two_sided = false;
        bool is_lit = true;
        Diffuse diffuse;
        Normal normal;
        Specular specular;
        Emissive emissive;

		Material(const Material&) = delete;
		Material& operator=(const Material&) = delete;

    };
}
