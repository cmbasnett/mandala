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
                data[y][x] = (static_cast<f32>(*data_iter) / 255.0f);

                data_iter += image->get_pixel_stride();
            }
        }
    }

	f32 heightmap::get_height(f32 x, f32 z) const
    {
        return 0.0f;
	}
	
	f32 heightmap::get_height(const vec2& location) const
	{
		return get_height(location.x, location.y);
	}
}