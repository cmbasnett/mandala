#pragma once

//std
#include <vector>

//mandala
#include "../mandala.h"
#include "../quake_camera.h"

//armada
#include "skybox.h"

namespace mandala
{
	namespace armada
	{
		struct world_t
		{
			quake_camera_t camera;
			skybox_t skybox;
			std::vector<std::shared_ptr<model_instance_t>> model_instances;

            world_t();

            void tick(float32_t dt);
            void render();
		};
	};
};
