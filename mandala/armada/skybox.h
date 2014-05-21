#pragma once

//std
#include <memory>

//memory
#include "../camera.h"

namespace mandala
{
	struct model_instance_t;

	namespace armada
	{
		struct skybox_t
		{
			std::shared_ptr<model_instance_t> model_instance;

			skybox_t();

			void render(const camera_t& camera_);

		private:
			skybox_t(const skybox_t&) = delete;
			skybox_t& operator=(const skybox_t&) = delete;
		};
	};
};
