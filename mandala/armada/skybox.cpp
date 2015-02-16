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
			model = std::make_shared<model_t>(hash_t("skybox.md5m"));
		}
		
		void skybox_t::render(const camera_t& camera)
		{
            model->pose.location = camera.location;
            model->render(camera, vec3_t(0));

            gpu.clear(gpu_t::clear_flag_depth);
		}
	};
};
