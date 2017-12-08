//naga
#include "gui_button.hpp"
#include "collision.hpp"
#include "input_event.hpp"

namespace naga
{
	bool GUIButton::on_input_event_begin(InputEvent& input_event)
    {
		if (input_event.device_type == InputEvent::DeviceType::TOUCH)
        {
            auto is_contained = contains(get_bounds(), input_event.touch.location);

            switch (input_event.touch.type)
            {
			case InputEvent::Touch::Type::PRESS:
                if (is_contained)
                {
                    state = State::PRESSED;

                    if (on_state_changed)
                    {
                        on_state_changed(shared_from_this());
                    }

                    return true;
                }
                break;
			case InputEvent::Touch::Type::RELEASE:
                if (is_contained && state == State::PRESSED)
                {
					state = State::HOVER;

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
			case InputEvent::Touch::Type::MOVE:
                switch (state)
                {
				case State::IDLE:
#if defined(NAGA_PC)
                    if (is_contained)
                    {
						state = State::HOVER;
                        
                        if (on_state_changed)
                        {
                            on_state_changed(shared_from_this());
                        }

                        return true;
                    }
#endif
                    break;
#if defined(NAGA_PC)
				case State::HOVER:
				case State::PRESSED:
                    if (!is_contained)
                    {
						state = State::IDLE;

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
