//glm
#include <glm\glm.hpp>
#include <glm\ext.hpp>

//mandala
#include "../opengl.hpp"
#include "../resource_mgr.hpp"
#include "../camera.hpp"
#include "../model.hpp"
#include "../model_info.hpp"

//armada
#include "skybox.hpp"

namespace mandala
{
	namespace armada
	{
		skybox_t::skybox_t()
		{
			const auto model = resources.get<model_info_t>(hash_t("skybox.md5m"));

			model_instance = std::make_shared<model_t>(model);
		}
		
		void skybox_t::render(const camera_t& camera)
		{
			model_instance->world_matrix = glm::translate(camera.location);

			model_instance->render(camera, vec3_t(0));

            gpu.clear(gpu_t::clear_flag_depth);
		}
	};
};
