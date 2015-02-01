//mandala
#include "gui_button.hpp"
#include "collision.hpp"
#include "input_event.hpp"

namespace mandala
{
    void gui_button_t::on_input_event(input_event_t& input_event)
    {
        if (input_event.device_type == input_event_t::device_type_e::touch)
        {
            switch (input_event.touch.type)
            {
            case input_event_t::touch_t::type_e::press:
                if (contains(get_bounds(), input_event.touch.position))
                {
                    _state = state_t::pressed;
                }
                break;
            case input_event_t::touch_t::type_e::release:
                if (_state == state_t::pressed && contains(get_bounds(), input_event.touch.position))
                {
                    _state = state_t::idle;

                    if (_pressed_function)
                    {
                        _pressed_function();
                    }
                }
                break;
            case input_event_t::touch_t::type_e::move:
                switch (_state)
                {
                case state_t::idle:
#if defined(MANDALA_PC)
                    if (contains(get_bounds(), input_event.touch.position))
                    {
                        _state = state_t::hover;
                        
                        if (_hover_function)
                        {
                            _hover_function();
                        }
                    }
#endif
                    break;
#if defined(MANDALA_PC)
                case state_t::hover:
                    if (!contains(get_bounds(), input_event.touch.position))
                    {
                        _state = state_t::idle;

                        if (_unhover_function)
                        {
                            _unhover_function();
                        }
                    }
                    break;
#endif
                default:
                    break;
                }
                break;
            }
        }
    }
}
