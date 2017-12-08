#pragma once

//naga
#include "resource.hpp"
#include "color_types.hpp"
#include "gpu_defs.hpp"

//boost
#include <boost\shared_ptr.hpp>
#include <boost\enable_shared_from_this.hpp>

namespace naga
{
    struct Image;

	struct Texture : Resource, boost::enable_shared_from_this<Texture>
    {
		typedef GpuId IdType;
        typedef i32 FormatType;
        typedef i32 TypeType;

		Texture(ColorType color_type, const vec2& size, const void* data);
		Texture(const boost::shared_ptr<Image>& image);
		Texture(std::istream& istream);
		virtual ~Texture();

		ColorType get_color_type() const { return color_type; }
		const vec2& get_size() const { return size; }
		const u32 get_width() const { return static_cast<u32>(size.x); }
		const u32 get_height() const { return static_cast<u32>(size.y); }

		IdType get_id() const { return id; }

		void set_size(const vec2& size);

    private:
		ColorType color_type;
		vec2 size;
		IdType id = 0;

		Texture(Texture&) = delete;
		Texture& operator=(Texture&) = delete;
    };
}
