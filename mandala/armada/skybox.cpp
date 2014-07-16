//glm
#include <glm\glm.hpp>
#include <glm\ext.hpp>

//mandala
#include "../mandala.hpp"
#include "../app.hpp"
#include "../camera.hpp"
#include "../model.hpp"
#include "../model_instance.hpp"

//armada
#include "skybox.hpp"

namespace mandala
{
	namespace armada
	{
		skybox_t::skybox_t()
		{
			auto model = app.resources.get<model_t>(hash_t("skybox.md5m"));
			model_instance = std::make_shared<model_instance_t>(model);
		}
		
		void skybox_t::render(const camera_t& camera)
		{
			model_instance->world_matrix = glm::translate(camera.position);

			model_instance->render(camera, vec3_t(0));

            //TODO: do through gpu_mgr
			glClear(GL_DEPTH_BUFFER_BIT);
		}
	};
};
