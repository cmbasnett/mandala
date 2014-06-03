//glm
#include "glm\ext.hpp"

//mandala
#include "../texture.hpp"
#include "../sprite_set.hpp"
#include "../sprite.hpp"
#include "gui_image.hpp"

namespace mandala
{
	namespace gui
	{
		void image_t::render(mat4_t world_matrix, mat4_t view_projection_matrix)
		{
			if (is_hidden)
			{
				return;
			}

			//push GL states
            auto is_blend_enabled = glIsEnabled(GL_BLEND); glCheckError();
			GLint blend_src_rgb;
			GLint blend_dst_alpha;
            auto is_line_stipple_enabled = glIsEnabled(GL_LINE_STIPPLE); glCheckError();
			GLint line_stipple_factor;
			GLint line_stipple_pattern;

            glGetIntegerv(GL_BLEND_SRC_RGB, &blend_src_rgb); glCheckError();
            glGetIntegerv(GL_BLEND_SRC_ALPHA, &blend_dst_alpha); glCheckError();
            glGetIntegerv(GL_LINE_STIPPLE_REPEAT, &line_stipple_factor); glCheckError();
            glGetIntegerv(GL_LINE_STIPPLE_PATTERN, &line_stipple_pattern); glCheckError();

            glUseProgram(0); glCheckError();

            glMatrixMode(GL_PROJECTION); glCheckError();
            glLoadMatrixf(glm::value_ptr(view_projection_matrix)); glCheckError();

			if (sprite.sprite_set->texture->has_alpha)
			{
                glEnable(GL_BLEND); glCheckError();
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glCheckError();
			}
			else
			{
                glDisable(GL_BLEND); glCheckError();
			}

			//auto min = bounds.min + (vec2_t)sprite.region.rectangle.min();
			//auto max = min + (vec2_t)sprite.region.rectangle.size();

			//aabb2_t aabb(min, max);

			//glLineStipple(1, 0xAAAA);
			//glEnable(GL_LINE_STIPPLE);

			//glBegin(GL_LINE_LOOP);
			//glColor3f(1, 1, 0);
			//glVertex2f(min.x, min.y);
			//glVertex2f(min.x, max.y);
			//glVertex2f(max.x, max.y);
			//glVertex2f(max.x, min.y);
			//glEnd();

            glEnable(GL_TEXTURE_2D); glCheckError();
            glActiveTexture(GL_TEXTURE0); glCheckError();
            glBindTexture(GL_TEXTURE_2D, sprite.sprite_set->texture->id); glCheckError();

            glDisable(GL_LINE_STIPPLE); glCheckError();

            glMatrixMode(GL_MODELVIEW); glCheckError();
			glPushMatrix(); glCheckError();

			auto center = bounds.min + ((vec2_t)sprite.region.rectangle.size() / 2.0f);
			auto size = (vec2_t)sprite.region.rectangle.size();
			auto min = -size / 2.0f;
			auto max = size / 2.0f;

			glTranslatef(center.x, center.y, 0.0f); glCheckError();
			glRotatef(rotation, 0, 0, 1); glCheckError();

			glBegin(GL_QUADS);

			glColor3f(1, 1, 1);

			//TODO: different texture coordinates based on uv parameters
			if (sprite.region.is_rotated)
			{
				glTexCoord2f(sprite.region.uv.min.x, sprite.region.uv.max.y);
				glVertex2f(min.x, min.y);
				glTexCoord2f(sprite.region.uv.min.x, sprite.region.uv.min.y);
				glVertex2f(max.x, min.y);
				glTexCoord2f(sprite.region.uv.max.x, sprite.region.uv.min.y);
				glVertex2f(max.x, max.y);
				glTexCoord2f(sprite.region.uv.max.x, sprite.region.uv.max.y);
				glVertex2f(min.x, max.y);
			}
			else
			{
				glTexCoord2f(sprite.region.uv.min.x, sprite.region.uv.min.y);
				glVertex2f(min.x, min.y);
				glTexCoord2f(sprite.region.uv.max.x, sprite.region.uv.min.y);
				glVertex2f(max.x, min.y);
				glTexCoord2f(sprite.region.uv.max.x, sprite.region.uv.max.y);
				glVertex2f(max.x, max.y);
				glTexCoord2f(sprite.region.uv.min.x, sprite.region.uv.max.y);
				glVertex2f(min.x, max.y);
			}

			glEnd();

			glPopMatrix();

			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);

			//blend
			if (is_blend_enabled)
			{
				glEnable(GL_BLEND);
			}
			else
			{
				glDisable(GL_BLEND);
			}

			//blend function
			glBlendFunc(blend_src_rgb, blend_dst_alpha);

			//line stipple
			if (is_line_stipple_enabled)
			{
				glEnable(GL_LINE_STIPPLE);
			}
			else
			{
				glDisable(GL_LINE_STIPPLE);
			}

			glLineStipple(line_stipple_factor, line_stipple_pattern);

			node_t::render(world_matrix, view_projection_matrix);
		}

		bool image_t::clean()
		{
			if (is_autosized_to_texture)
			{
				size = static_cast<vec2_t>(sprite.region.source_size);
			}

			return node_t::clean();
		}
	};
};
