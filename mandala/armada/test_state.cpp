//std
#include <iostream>

//mandala
#include "../input_event.h"
#include "../app.h"

//armada
#include "test_state.h"

namespace mandala
{
	namespace armada
	{
		test_state_t::test_state_t(uint64_t index) :
			index(index)
		{
			link_flags = link_flag_all;
		}

		void test_state_t::on_input_event(input_event_t& input_event)
		{
			if (input_event.device_type == input_event_t::device_type_t::keyboard &&
				input_event.keyboard.type == input_event_t::keyboard_t::type_t::key_press)
			{
				switch (input_event.keyboard.key)
				{
					case input_event_t::keyboard_t::key_t::escape:
					{
						std::cout << "---------------------------------------------------------------" << std::endl;
						std::cout << "[" << index << "] pop" << std::endl;
						std::cout << "---------------------------------------------------------------" << std::endl;
						app.states.pop(shared_from_this());
						input_event.is_consumed = true;
						break;
					}
					case input_event_t::keyboard_t::key_t::enter:
					{
						std::cout << "---------------------------------------------------------------" << std::endl;
						std::cout << "[" << index + 1 << "] push" << std::endl;
						std::cout << "---------------------------------------------------------------" << std::endl;
						app.states.push(std::make_shared<test_state_t>(index + 1));
						input_event.is_consumed = true;
						break;
					}
				}
			}
		}

		void test_state_t::on_active()
		{
			std::cout << "[" << index << "] on_active" << std::endl;
		}

		void test_state_t::on_passive()
		{
			std::cout << "[" << index << "] on_passive" << std::endl;
		}

		void test_state_t::on_enter()
		{
			std::cout << "[" << index << "] on_enter" << std::endl;
		}

		void test_state_t::on_exit()
		{
			std::cout << "[" << index << "] on_exit" << std::endl;
		}

		void test_state_t::on_stop_input()
		{
			std::cout << "[" << index << "] on_stop_input" << std::endl;
		}

		void test_state_t::on_start_input()
		{
			std::cout << "[" << index << "] on_start_input" << std::endl;
		}
	};
};
