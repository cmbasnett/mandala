#pragma once

//boost
#include <boost\shared_ptr.hpp>

//mandala
#include "../camera.hpp"

namespace mandala
{
	struct model_instance_t;

	namespace armada
	{
		struct skybox_t
		{
			boost::shared_ptr<model_instance_t> model;

			skybox_t();

			void render(const camera_t& camera);

		private:
			skybox_t(const skybox_t&) = delete;
			skybox_t& operator=(const skybox_t&) = delete;
		};
	};
};
