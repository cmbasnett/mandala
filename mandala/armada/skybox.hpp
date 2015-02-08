#pragma once

//std
#include <memory>

//memory
#include "../camera.hpp"

namespace mandala
{
	struct model_t;

	namespace armada
	{
		struct skybox_t
		{
			std::shared_ptr<model_t> model_instance;

			skybox_t();

			void render(const camera_t& camera);

		private:
			skybox_t(const skybox_t&) = delete;
			skybox_t& operator=(const skybox_t&) = delete;
		};
	};
};
