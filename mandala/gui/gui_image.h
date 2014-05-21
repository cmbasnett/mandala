#pragma once

//mandala
#include "../sprite.h"
#include "node.h"

namespace mandala
{
	namespace gui
	{
		struct image_t : public node_t
		{
			sprite_t sprite;
			bool is_autosized_to_texture = false;
			int32_t vertex_buffer = 0;
			int32_t index_buffer = 0;

			virtual void render(mat4_t world_matrix, mat4_t view_projection_matrix);
			virtual bool clean();
		};
	};
};
