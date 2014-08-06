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
		std::shared_ptr<state_t> previous_top_state = nodes.empty() ? nullptr : nodes.rbegin()->state;

		//process stack operations
		while (!operations.empty())
		{
			const auto& operation = operations.front();

			auto nodes_itr = std::find_if(nodes.begin(), nodes.end(), [&](const node_t& node)
			{
				return node.state == operation.state;
			});

			switch (operation.type)
			{
			case operation_t::type_e::pop:
				if (nodes_itr == nodes.end())
				{
					//state does not exist, cannot pop
					throw std::exception();
				}

				//call on_exit on popped state
				nodes_itr->state->on_exit();

				//remove state from stack
				nodes_itr = nodes.erase(nodes_itr);

				break;
			case operation_t::type_e::push:
				if (nodes_itr != nodes.end())
				{
					//state already exists on the stack, cannot push
					throw std::exception();
				}

				//call on_enter on pushed state
				operation.state->on_enter();

				//add state to stack
				node_t node;
				node.link_flags = operation.link_flags;
				node.state = operation.state;

				nodes.push_back(node);

				break;
			}

			operations.pop_front();
		}

		//check if top state changed
		if (previous_top_state != nodes.rbegin()->state)
		{
			//call on_passive on previous top state
			if (previous_top_state != nullptr)
			{
				previous_top_state->on_passive();
			}

			//call on_active on new top state
			if (!nodes.empty())
			{
				nodes.rbegin()->state->on_active();
			}
		}

		//compare previous node flags to current ones, fire off start/top flag events to states
		flags_type flags = flag_all;

		

		for (auto nodes_reverse_itr = nodes.rbegin(); nodes_reverse_itr != nodes.rend(); ++nodes_reverse_itr)
		{
			auto previous_node_flags = nodes_reverse_itr->flags;

			nodes_reverse_itr->flags = flags;

			flags &= nodes_reverse_itr->flags;

			//render
			if ((previous_node_flags & flag_render) == flag_none && (flags & flag_render) == flag_render)
			{
				nodes_reverse_itr->state->on_start_render();
			}
			else if ((previous_node_flags & flag_render) == flag_render && (flags & flag_render) == flag_none)
			{
				nodes_reverse_itr->state->on_stop_render();
			}

			//tick
			if ((previous_node_flags & flag_tick) == flag_none && (flags & flag_tick) == flag_tick)
			{
				nodes_reverse_itr->state->on_start_tick();
			}
			else if ((previous_node_flags & flag_tick) == flag_tick && (flags & flag_tick) == flag_none)
			{
				nodes_reverse_itr->state->on_stop_tick();
			}

			//input
			if ((previous_node_flags & flag_input) == flag_none && (flags & flag_input) == flag_input)
			{
				nodes_reverse_itr->state->on_start_input();
			}
			else if ((previous_node_flags & flag_input) == flag_input && (flags & flag_input) == flag_none)
			{
				nodes_reverse_itr->state->on_stop_input();
			}
		}

		//tick states
		for (auto nodes_reverse_itr = nodes.rbegin(); nodes_reverse_itr != nodes.rend(); ++nodes_reverse_itr)
		{
			if ((nodes_reverse_itr->flags & flag_tick) == flag_none)
			{
				//state not ticking, break
				break;
			}

			nodes_reverse_itr->state->tick(dt);
		}
	}

	void state_mgr_t::render()
	{
		flags_type link_flags = flag_all;

		//find first renderable state
		auto nodes_reverse_itr = nodes.rbegin();

		while (nodes_reverse_itr != nodes.rend())
		{
			if ((link_flags & flag_render) == flag_none)
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
		flags_type link_flags = flag_all;

		for (auto nodes_itr = nodes.rbegin(); nodes_itr != nodes.rend(); ++nodes_itr)
		{
			if ((link_flags & flag_input) == flag_none)
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
	void state_mgr_t::push(const std::shared_ptr<state_t>& state, flags_type link_flags)
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

	size_t state_mgr_t::count() const
	{
		return nodes.size();
	}
}
