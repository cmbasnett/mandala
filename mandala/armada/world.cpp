//glm
#include <glm\gtc\matrix_transform.hpp>

//mandala
#include "../app.h"
#include "../model.h"
#include "../model_instance.h"

//armada
#include "world.h"

namespace mandala
{
	namespace armada
	{
        world_t::world_t()
		{
			auto model = app.resources.get<model_t>(hash_t("space_fighter_6.md5m"));
			auto model_instance = std::make_shared<model_instance_t>(model);
			model_instances.push_back(model_instance);
        }

		void world_t::tick(float32_t dt)
		{
			camera.tick(dt);

			for(auto& model_instance : model_instances)
			{
				model_instance->tick(dt);
			}
		}

		void world_t::render()
		{
			skybox.render(camera);

			auto light_position = vec3_t(0, 20, 100);

			for(auto& model_instance : model_instances)
			{
				model_instance->render(camera, light_position);
			}
		}
	};
};
