#include "state_mgr.hpp"
#include "state.hpp"
#include "app.hpp"

namespace mandala
{
	state_mgr_t::state_mgr_t()
	{
	}

	void state_mgr_t::tick(float32_t dt)
	{
		//process stack actions
		while (operation_queue.size() > 0)
		{
			const auto& stack_action = operation_queue.front();

			switch (stack_action.type)
			{
			case operation_t::type_t::pop:
				{
					auto states_itr = std::find(states.begin(), states.end(), stack_action.state);

					if (states_itr == states.end())
					{
						//state does not exist, cannot pop
						throw std::exception();
					}

					auto state_reverse_itr = std::reverse_iterator<decltype(states_itr)>(states_itr);

					if (state_reverse_itr != states.rend())
					{
						(*state_reverse_itr)->on_active();
					}

					while (state_reverse_itr != states.rend())
					{
						(*state_reverse_itr)->on_start_input();

						if (((*state_reverse_itr)->link_flags & state_t::link_flag_input) == 0)
						{
							break;
						}

						++state_reverse_itr;
					}

					//remove state from stack
					states.erase(states_itr, states_itr + 1);
				}
				break;
			case operation_t::type_t::push:
				{
					auto push_state_itr = std::find(states.begin(), states.end(), stack_action.state);

					if(push_state_itr != states.end())
					{
						//state already exists on the stack, cannot push
						throw std::exception();
					}
					
					if(!states.empty())
					{
						auto states_itr = states.rbegin();

						//call on_passive on previous top state
						(*states_itr)->on_passive();

						if((stack_action.state->link_flags & state_t::link_flag_input) == 0)
						{
							//pushed state is now blocking input to lower states
							//cascade on_stop_input to top states that were accepting input
							for (auto states_itr = states.rbegin(); states_itr != states.rend(); ++states_itr)
							{
								(*states_itr)->on_stop_input();

								if (((*states_itr)->link_flags & state_t::link_flag_input) == 0)
								{
									break;
								}
							}
						}
					}

					//add state to stack
					states.push_back(stack_action.state);

					//call on_enter, on_active etc. on pushed state
					stack_action.state->on_enter();
					stack_action.state->on_active();
					stack_action.state->on_start_tick();
					stack_action.state->on_start_render();
					stack_action.state->on_start_input();
				}
				break;
			}

			operation_queue.pop_front();
		}

		uint8_t link_flags = state_t::link_flag_all;
		
		for (auto states_itr = states.rbegin(); states_itr != states.rend(); ++states_itr)
		{
			auto state = (*states_itr);

			if((link_flags & state_t::link_flag_tick) == 0)
			{
				//state not ticking, break
				break;
			}

			state->tick(dt);

			link_flags &= state->link_flags;
		}
	}

	void state_mgr_t::render()
	{
		state_t::link_flags_t link_flags = state_t::link_flag_all;
		
		//find first renderable state
		auto state_reverse_itr = states.rbegin();

		while(state_reverse_itr != states.rend())
		{
			if((link_flags & state_t::link_flag_render) == 0)
			{
				break;
			}

			auto state = (*state_reverse_itr);

			link_flags &= state->link_flags;

			++state_reverse_itr;
		}

		for(auto state_itr = state_reverse_itr.base(); state_itr != states.end(); ++state_itr)
		{
			auto state = (*state_itr);

			state->render();
		}
	}

	void state_mgr_t::on_input_event(input_event_t& input_event)
	{
		state_t::link_flags_t link_flags = state_t::link_flag_all;
		
		for(auto state_it = states.rbegin(); state_it != states.rend(); ++state_it)
		{
			if((link_flags & state_t::link_flag_input) == 0)
			{
				//state not handling input, return
				return;
			}

			auto state = (*state_it);

			state->on_input_event(input_event);

			if (input_event.is_consumed)
			{
				//state consumed input event, return
				return;
			}

			link_flags &= state->link_flags;
		}
	}

	//push a state onto the stack
	void state_mgr_t::push(std::shared_ptr<state_t> state)
	{
		if (state == nullptr)
		{
			throw std::exception();
		}

		operation_t operation;
		operation.type = operation_t::type_t::push;
		operation.state = state;

		operation_queue.push_back(operation);
	}

	//pop a specific state off of the stack
	void state_mgr_t::pop(std::shared_ptr<state_t> state)
	{
		if (state == nullptr)
		{
			throw std::exception();
		}

		operation_t operation;
		operation.type = operation_t::type_t::pop;
		operation.state = state;

		operation_queue.push_back(operation);
	}

	void state_mgr_t::purge()
	{
		states.clear();
	}
};
