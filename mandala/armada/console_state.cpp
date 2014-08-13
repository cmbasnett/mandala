//mandala
#include "../app.hpp"
#include "../bitmap_font.hpp"

//armada
#include "console_state.hpp"

//boost
#include <boost\algorithm\string.hpp>

namespace mandala
{
	namespace armada
	{
		console_state_t::console_state_t()
		{
			root_node = std::make_shared<gui_node_t>();
			root_node->dock_mode = gui_dock_mode_e::top;
			root_node->size.y = layout->bounds.size().y / 2;

			input_label = std::make_shared<gui_label_t>();
			input_label->bitmap_font = app.resources.get<bitmap_font_t>(hash_t("terminal_16.fnt"));
			input_label->size.y = input_label->bitmap_font->line_height;
			input_label->dock_mode = gui_dock_mode_e::bottom;
			input_label->padding = padding_t(16);
			input_label->string = L"test";

			output_label = std::make_shared<gui_label_t>();
			output_label->bitmap_font = app.resources.get<bitmap_font_t>(hash_t("terminal_16.fnt"));
			output_label->dock_mode = gui_dock_mode_e::fill;
			output_label->padding = padding_t(16);
			output_label->vertical_alignment = gui_label_t::vertical_alignment_e::bottom;
			output_label->justification = gui_label_t::justification_e::left;
			output_label->string = L"test";

			root_node->adopt(input_label);
			root_node->adopt(output_label);

			layout->adopt(root_node);

			layout->put<gui_label_t>(hash_t("input_label"), input_label);
			layout->put<gui_label_t>(hash_t("output_label"), output_label);

			std::wstring name;
			boost::replace_all(input_label->string, "#name1", name);
			boost::replace_all(input_label->string, "#name2", name);

			layout->clean();
		}
	}
}
