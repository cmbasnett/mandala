//glm
#include <glm\glm.hpp>
#include <glm\ext.hpp>

//mandala
#include "../app.h"
#include "../camera.h"
#include "../model.h"
#include "../model_instance.h"

//armada
#include "skybox.h"

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
			model_instance->world_matrix = glm::translate(camera.location);

			model_instance->render(camera, vec3_t(0));

			glClear(GL_DEPTH_BUFFER_BIT);
		}
	};
};
