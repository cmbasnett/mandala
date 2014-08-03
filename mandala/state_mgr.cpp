//mandala
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
		while (!operations.empty())
		{
			const auto& operation = operations.front();

			auto nodes_itr = std::find_if(nodes.begin(), nodes.end(), [&](const node_t& node)
			{
				return node.state == operation.state;
			});

			auto nodes_reverse_itr = std::reverse_iterator<decltype(nodes_itr)>(nodes_itr);

			switch (operation.type)
			{
			case operation_t::type_e::pop:

				if (nodes_itr == nodes.end())
				{
					//state does not exist, cannot pop
					throw std::exception();
				}

				if (nodes_reverse_itr != nodes.rend())
				{
					nodes_reverse_itr.base()->state->on_active();
				}

				while (nodes_reverse_itr != nodes.rend())
				{
					nodes_reverse_itr->state->on_start_input();

					if ((nodes_reverse_itr->link_flags & link_flag_input) == 0)
					{
						break;
					}

					++nodes_reverse_itr;
				}

				//remove state from stack
				nodes.erase(nodes_itr);

				break;
			case operation_t::type_e::push:
				if (nodes_itr != nodes.end())
				{
					//state already exists on the stack, cannot push
					throw std::exception();
				}

				if (!nodes.empty())
				{
					auto nodes_itr = nodes.rbegin();

					//call on_passive on previous top state
					nodes_itr->state->on_passive();

					if ((operation.link_flags & link_flag_input) == link_flag_none)
					{
						//pushed state is now blocking input to lower states
						//cascade on_stop_input to top states that were accepting input
						for (auto nodes_itr = nodes.rbegin(); nodes_itr != nodes.rend(); ++nodes_itr)
						{
							nodes_itr->state->on_stop_input();

							if ((nodes_itr->link_flags & link_flag_input) == link_flag_none)
							{
								break;
							}
						}
					}
				}

				//add state to stack
				node_t node;
				node.link_flags = operation.link_flags;
				node.state = operation.state;

				nodes.push_back(node);

				//call on_enter, on_active etc. on pushed state
				operation.state->on_enter();
				operation.state->on_active();
				operation.state->on_start_tick();
				operation.state->on_start_render();
				operation.state->on_start_input();

				break;
			}

			operations.pop_front();
		}

		link_flags_type link_flags = link_flag_all;

		for (auto nodes_itr = nodes.rbegin(); nodes_itr != nodes.rend(); ++nodes_itr)
		{
			const auto& state = nodes_itr->state;

			if ((link_flags & link_flag_tick) == link_flag_none)
			{
				//state not ticking, break
				break;
			}

			state->tick(dt);

			link_flags &= nodes_itr->link_flags;
		}
	}

	void state_mgr_t::render()
	{
		link_flags_type link_flags = link_flag_all;

		//find first renderable state
		auto nodes_reverse_itr = nodes.rbegin();

		while (nodes_reverse_itr != nodes.rend())
		{
			if ((link_flags & link_flag_render) == link_flag_none)
			{
				break;
			}

			link_flags &= nodes_reverse_itr->link_flags;

			++nodes_reverse_itr;
		}

		for (auto nodes_itr = nodes_reverse_itr.base(); nodes_itr != nodes.end(); ++nodes_itr)
		{
			nodes_itr->state->render();
		}
	}

	void state_mgr_t::on_input_event(input_event_t& input_event)
	{
		link_flags_type link_flags = link_flag_all;

		for (auto nodes_itr = nodes.rbegin(); nodes_itr != nodes.rend(); ++nodes_itr)
		{
			if ((link_flags & link_flag_input) == link_flag_none)
			{
				//state not handling input, return
				return;
			}

			auto& state = nodes_itr->state;

			state->on_input_event(input_event);

			if (input_event.is_consumed)
			{
				//state consumed input event, return
				return;
			}

			link_flags &= nodes_itr->link_flags;
		}
	}

	//push a state onto the stack
	void state_mgr_t::push(const std::shared_ptr<state_t>& state, link_flags_type link_flags)
	{
		if (state == nullptr)
		{
			throw std::exception();
		}

		operation_t operation;
		operation.type = operation_t::type_e::push;
		operation.state = state;
		operation.link_flags = link_flags;

		operations.push_back(operation);
	}

	//pop a specific state off of the stack
	void state_mgr_t::pop(const std::shared_ptr<state_t>& state)
	{
		if (state == nullptr)
		{
			throw std::exception();
		}

		operation_t operation;
		operation.type = operation_t::type_e::pop;
		operation.state = state;

		operations.push_back(operation);
	}

	void state_mgr_t::purge()
	{
		nodes.clear();
	}
};
