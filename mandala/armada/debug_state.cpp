//mandala
#include "../resource_mgr.hpp"
#include "../bitmap_font.hpp"
#include "../texture.hpp"
#include "../sound.hpp"
#include "../image.hpp"
#include "../model.hpp"
#include "../sprite_set.hpp"
#include "../material.hpp"
#include "../color.hpp"
#include "../audio_mgr.hpp"
#include "../app.hpp"
#include "../state_mgr.hpp"

//armada
#include "debug_state.hpp"

//std
#include <sstream>
#include <codecvt>

namespace mandala
{
	debug_state_t::debug_state_t()
	{
		label = std::make_shared<gui_label_t>();
		label->set_bitmap_font(resources.get<bitmap_font_t>(hash_t("inconsolata_12.fnt")));
		label->set_dock_mode(gui_dock_mode_e::fill);
		label->set_vertical_alignment(gui_label_t::vertical_alignment_e::bottom);
		label->set_justification(gui_label_t::justification_e::right);
		label->set_padding(padding_t(16));

		layout->adopt(label);
	}

	void debug_state_t::tick(float32_t dt)
	{
        std::wstring_convert<std::codecvt_utf8<wchar_t>> wstring_convert;

		std::wostringstream oss;

		oss << wstring_convert.from_bytes(gpu.get_vendor()) << std::endl;
		oss << wstring_convert.from_bytes(gpu.get_renderer()) << std::endl;
		oss << wstring_convert.from_bytes(gpu.get_shading_language_version()) << std::endl;
		oss << wstring_convert.from_bytes(gpu.get_version()) << std::endl;
		oss << std::endl;
		oss << "[audio]" << std::endl;
		oss << "sources: " << audio.get_sources().size() << std::endl;
		oss << std::endl;
		oss << "[performance]" << std::endl;
		oss << "fps: ";

		if (app.get_performance().fps >= 30)
		{
			oss << color_push(color_green);
		}
		else if (app.get_performance().fps >= 25)
		{
			oss << color_push(color_yellow);
		}
		else
		{
			oss << color_push(color_red);
		}

		oss << static_cast<int32_t>(app.get_performance().fps);
		oss << color_pop << std::endl;
		oss << "[resources]" << std::endl;
		oss << "bitmap fonts: " << resources.count<bitmap_font_t>() << std::endl;
		oss << "textures: " << resources.count<texture_t>() << std::endl;
		oss << "sounds: " << resources.count<sound_t>() << std::endl;
		oss << "images: " << resources.count<image_t>() << std::endl;
		oss << "models: " << resources.count<model_info_t>() << std::endl;
		oss << "sprite sets: " << resources.count<sprite_set_t>() << std::endl;
		oss << "materials: " << resources.count<material_t>() << std::endl;
		oss << "total: " << resources.count() << std::endl;
		oss << std::endl;
		oss << "[states]" << std::endl;
		oss << states.count() << std::endl;

		label->set_string(oss.str());

		gui_state_t::tick(dt);
	}
}
