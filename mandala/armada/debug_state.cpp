//mandala
#include "../app.hpp"
#include "../bitmap_font.hpp"
#include "../texture.hpp"
#include "../sound.hpp"
#include "../image.hpp"
#include "../model.hpp"
#include "../sprite_set.hpp"
#include "../material.hpp"

//armada
#include "debug_state.hpp"

//std
#include <sstream>

namespace mandala
{
	debug_state_t::debug_state_t()
	{
		label = std::make_shared<gui_label_t>();
		label->bitmap_font = app.resources.get<bitmap_font_t>(hash_t("terminal_8.fnt"));
		label->dock_mode = gui_node_t::dock_mode_e::fill;
		label->vertical_alignment = gui_label_t::vertical_alignment_e::bottom;
		label->justification = gui_label_t::justification_e::right;
		label->should_use_color_codes = false;
		label->padding = padding_t(16);

		layout->adopt(label);

		layout->clean();
	}

	void debug_state_t::tick(float32_t dt)
	{
		std::wostringstream oss;

		oss << L"debug test↑0cool↓" << std::endl;
		oss << "[audio]" << std::endl;
		oss << "sources: " << app.audio.sources.size() << std::endl;
		oss << std::endl;
		oss << "[performance]" << std::endl;
		oss << "fps: " << static_cast<int32_t>(app.performance.fps) << std::endl;
		oss << std::endl;
		oss << "[resources]" << std::endl;
		oss << "bitmap fonts: " << app.resources.count<bitmap_font_t>() << std::endl;
		oss << "textures: " << app.resources.count<texture_t>() << std::endl;
		oss << "sounds: " << app.resources.count<sound_t>() << std::endl;
		oss << "images: " << app.resources.count<image_t>() << std::endl;
		oss << "models: " << app.resources.count<model_t>() << std::endl;
		oss << "sprite sets: " << app.resources.count<sprite_set_t>() << std::endl;
		oss << "materials: " << app.resources.count<material_t>() << std::endl;
		oss << "total: " << app.resources.count() << std::endl;
		oss << std::endl;
		oss << "[states]" << std::endl;
		oss << app.states.count() << std::endl;

		label->string = oss.str();

		layout->clean();
	}
}
