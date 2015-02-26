//mandala
#include "gui_element.hpp"
#include "gui_image.hpp"

namespace mandala
{
    gui_element_t::gui_element_t()
    {
        set_size(vec2_t(128));
        set_padding(padding_t(8));
        set_dock_mode(gui_dock_mode_e::left);

        auto image = std::make_shared<gui_image_t>();
        image->set_dock_mode(gui_dock_mode_e::fill);
        image->set_sprite(sprite_t(hash_t("crono.tpsb"), hash_t("chrono_08.gif")));
        image->set_is_autosized_to_texture(true);

        adopt(image);
    }
}
