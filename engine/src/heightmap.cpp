//std
#include <array>

//naga
#include "heightmap.hpp"
#include "triangle.hpp"
#include "collision.hpp"
#include "image.hpp"

namespace naga
{
    heightmap::heightmap(const boost::shared_ptr<image>& image) :
        heightmap(image->get_size().x, image->get_size().y)
    {
        auto data_iter = image->get_data().begin();

        for (u32 y = 0; y < image->get_height(); ++y)
        {
			for (u32 x = 0; x < image->get_width(); ++x)
            {
                inner[y][x] = (static_cast<f32>(*data_iter) / 255.0f) * 32.0f;

                data_iter += image->get_pixel_stride();
            }
        }

		for (u32 y = 1; y < image->get_height() - 1; ++y)
		{
			for (u32 x = 1; x < image->get_width() - 1; ++x)
			{
				outer[y][x] = (
					inner[y - 1][x - 1] +
					inner[y - 1][x + 0] +
					inner[y + 0][x - 1] +
					inner[y + 0][x + 0]) / 4;
			}
		}
    }

	f32 heightmap::get_outer_height(u32 x, u32 z) const
    {
        return outer[x][z];
    }

	f32 heightmap::get_inner_height(u32 x, u32 z) const
    {
        return inner[x][z];
    }

	f32 heightmap::get_height(f32 x, f32 z) const
    {
		auto x_index = glm::clamp(static_cast<u32>(glm::max(x, 0.0f)), 0U, width - 1);
		auto z_index = glm::clamp(static_cast<u32>(glm::max(z, 0.0f)), 0U, depth - 1);

        // z
        // ^ 3---4
        // | | 2 |
        // | 0---1
        // +---- > x
        const std::array<f32, 5> heights = {
            outer[x_index + 0][z_index + 0],
            outer[x_index + 1][z_index + 0],
            inner[x_index + 0][z_index + 0],
            outer[x_index + 0][z_index + 1],
            outer[x_index + 1][z_index + 1]
        };

        const std::array<vec3, 5> vertices = {
            vec3(0.0f, heights[0], 0.0f),
            vec3(1.0f, heights[1], 0.0f),
            vec3(0.5f, heights[2], 0.5f),
            vec3(0.0f, heights[3], 1.0f),
            vec3(1.0f, heights[4], 1.0f)
        };

        // z
        // ^ +-----+
        // | |\ 2 /|
        // | | \ / |
        // | |3 x 1|
        // | | / \ |
        // | |/ 0 \|
        // | +-----+
        // +------ > x

        const std::array<triangle3, 4> triangles = {
            triangle3(vertices[0], vertices[1], vertices[2]),
            triangle3(vertices[1], vertices[4], vertices[2]),
            triangle3(vertices[4], vertices[3], vertices[2]),
            triangle3(vertices[3], vertices[0], vertices[2])
        };

        auto x_whole = 0.0f;
        auto z_whole = 0.0f;
        const auto x_fractional = glm::modf(x, x_whole);
        const auto z_fractional = glm::modf(z, z_whole);

        const auto fractional = vec2(x_fractional, z_fractional);

        if (fractional.x > fractional.y)
        {
			static auto t = triangle2(vec2(0), vec2(1, 0), vec2(1, 1));

			auto bary = barycentric(fractional, t);
        }
        else
        {
            // 2 or 3
        }

        // Determine which triangle the location is in by getting the angle
        // between the point and the [0,4] vector.
        const auto theta = glm::acos(glm::dot(fractional_normalized, glm::normalize(vec2( 1.0f))));
        const auto iota  = glm::acos(glm::dot(fractional_normalized, glm::normalize(vec2(-1.0f))));

        size_t triangle_index = 0;

        if (theta > 0.0f)
        {
            if (iota > 0.0f)
            {
                triangle_index = 0;
            }
            else
            {
                triangle_index = 1;
            }
        }
        else
        {
            if (iota > 0.0f)
            {
                triangle_index = 2;
            }
            else
            {
                triangle_index = 3;
            }
        }
        
        return 0.0f;
	}
	
	f32 heightmap::get_height(const vec2& location) const
	{
		return get_height(location.x, location.y);
	}
}