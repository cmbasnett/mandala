//mandala
#include "gui_button.hpp"
#include "collision.hpp"
#include "input_event.hpp"

namespace mandala
{
    bool gui_button_t::on_input_event(input_event_t& input_event)
    {
        if (input_event.device_type == input_event_t::device_type_e::TOUCH)
        {
            switch (input_event.touch.type)
            {
            case input_event_t::touch_t::type_e::PRESS:
                if (contains(get_bounds(), input_event.touch.location))
                {
                    state = state_t::PRESSED;

                    return true;
                }
                break;
            case input_event_t::touch_t::type_e::RELEASE:
                if (state == state_t::PRESSED && contains(get_bounds(), input_event.touch.location))
                {
                    state = state_t::IDLE;

                    if (on_pressed)
                    {
                        on_pressed();
                    }

                    return true;
                }
                break;
            case input_event_t::touch_t::type_e::MOVE:
                switch (state)
                {
                case state_t::IDLE:
#if defined(MANDALA_PC)
                    if (contains(get_bounds(), input_event.touch.location))
                    {
                        state = state_t::HOVER;
                        
                        if (on_hover)
                        {
                            on_hover();
                        }

                        return true;
                    }
#endif
                    break;
#if defined(MANDALA_PC)
                case state_t::HOVER:
                    if (!contains(get_bounds(), input_event.touch.location))
                    {
                        state = state_t::IDLE;

                        if (on_unhover)
                        {
                            on_unhover();
                        }

                        return true;
                    }
                    break;
#endif
                default:
                    break;
                }
                break;
            }
        }

        return false;
    }
}
