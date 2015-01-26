//std
#include <iostream>

//mandala
#include "../input_event.hpp"
#include "../state_mgr.hpp"

//armada
#include "test_state.hpp"

namespace mandala
{
	namespace armada
	{
		test_state_t::test_state_t(uint64_t index) :
			index(index)
		{
		}

		void test_state_t::on_input_event(input_event_t& input_event)
		{
			if (input_event.device_type == input_event_t::device_type_e::keyboard &&
				input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_press)
			{
				switch (input_event.keyboard.key)
				{
					case input_event_t::keyboard_t::key_e::escape:
					{
						std::cout << "---------------------------------------------------------------" << std::endl;
						std::cout << "[" << index << "] pop" << std::endl;
						std::cout << "---------------------------------------------------------------" << std::endl;
						states.pop(shared_from_this());
						input_event.is_consumed = true;
						break;
					}
					case input_event_t::keyboard_t::key_e::enter:
					{
						std::cout << "---------------------------------------------------------------" << std::endl;
						std::cout << "[" << index + 1 << "] push" << std::endl;
						std::cout << "---------------------------------------------------------------" << std::endl;
						states.push(std::make_shared<test_state_t>(index + 1), state_flag_all);
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
