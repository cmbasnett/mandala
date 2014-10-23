//mandala
#include "performance_state.h"
#include "app.h"
#include "platform.h"

//glm
#include <glm\ext.hpp>

namespace mandala
{
	namespace armada
	{
		performance_state_t::performance_state_t()
		{
			link_flags = state_t::LINK_FLAG_ALL;
		}

		void performance_state_t::render()
		{
			auto bitmap_font = app.bitmap_font_mgr.get("consolas_32.fnt").lock();

			vec4_t viewport;
			glGetFloatv(GL_VIEWPORT, glm::value_ptr(viewport));

			auto string_size = bitmap_font->measure_string("paused");

			auto view_projection = glm::ortho(viewport.x, viewport.z, viewport.y, viewport.w);
			auto world_matrix = glm::translate(viewport.z / 2.0f, viewport.w / 2.0f, 0.0f);

			bitmap_font->render_string("paused", world_matrix, view_projection);
		}
	};
};
