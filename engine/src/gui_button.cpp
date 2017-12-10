//naga
#include "gui_button.hpp"
#include "collision.hpp"
#include "input_event.hpp"

namespace naga
{
	bool GUIButton::on_input_event_begin(InputEvent& input_event)
    {
		if (input_event.device_type == InputDeviceType::MOUSE)
        {
			auto is_contained = contains(get_bounds(), input_event.mouse.location);

            switch (input_event.mouse.type)
            {
			case InputEvent::Mouse::Type::PRESS:
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
			case InputEvent::Mouse::Type::RELEASE:
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
			case InputEvent::Mouse::Type::MOVE:
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
