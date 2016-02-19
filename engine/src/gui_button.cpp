//naga
#include "gui_button.hpp"
#include "collision.hpp"
#include "input_event.hpp"

namespace naga
{
    bool gui_button::on_input_event_begin(input_event_t& input_event)
    {
        if (input_event.device_type == input_event_t::device_type_e::TOUCH)
        {
            auto is_contained = contains(get_bounds(), input_event.touch.location);

            switch (input_event.touch.type)
            {
            case input_event_t::touch_t::type_e::PRESS:
                if (is_contained)
                {
                    state = state_t::PRESSED;

                    if (on_state_changed)
                    {
                        on_state_changed(shared_from_this());
                    }

                    return true;
                }
                break;
            case input_event_t::touch_t::type_e::RELEASE:
                if (is_contained && state == state_t::PRESSED)
                {
                    state = state_t::HOVER;

                    if (on_state_changed)
                    {
                        on_state_changed(shared_from_this());
                    }

					if (on_press)
					{
						on_press(shared_from_this());
					}

                    return true;
                }
                break;
            case input_event_t::touch_t::type_e::MOVE:
                switch (state)
                {
                case state_t::IDLE:
#if defined(NAGA_PC)
                    if (is_contained)
                    {
                        state = state_t::HOVER;
                        
                        if (on_state_changed)
                        {
                            on_state_changed(shared_from_this());
                        }

                        return true;
                    }
#endif
                    break;
#if defined(NAGA_PC)
                case state_t::HOVER:
                case state_t::PRESSED:
                    if (!is_contained)
                    {
                        state = state_t::IDLE;

                        if (on_state_changed)
                        {
                            on_state_changed(shared_from_this());
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
