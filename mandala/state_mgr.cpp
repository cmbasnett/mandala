//mandala
#include "state_mgr.hpp"
#include "state.hpp"
#include "app.hpp"

namespace mandala
{
	void state_mgr_t::tick(float32_t dt)
	{
		bool did_nodes_change = !operations.empty();

		state_type previous_top_state;

		if (!operations.empty())
		{
			previous_top_state = nodes.empty() ? nullptr : nodes.rbegin()->state;
		}

		std::vector<state_type> pushed_states;
		std::vector<state_type> popped_states;

		//process stack operations
		while (!operations.empty())
		{
			const auto& operation = operations.front();

			const auto nodes_itr = std::find_if(nodes.begin(), nodes.end(), [&](const node_t& node)
			{
				return node.state == operation.state;
			});

			switch (operation.type)
			{
			case operation_t::type_e::pop:
			{
			    if (nodes_itr == nodes.end())
			    {
				    throw std::exception("state does not exist on stack, cannot pop");
			    }

			    popped_states.push_back(nodes_itr->state);

			    //remove state from stack
                nodes.erase(nodes_itr);
			}
				break;
			case operation_t::type_e::push:
			{
		        if (nodes_itr != nodes.end())
		        {
			        throw std::exception("state already exists on stack, cannot push");
		        }

		        //add state to stack
		        node_t node;
		        node.link_flags = operation.link_flags;
		        node.state = operation.state;

		        nodes.push_back(node);

                pushed_states.push_back(operation.state);
			}
				break;
			case operation_t::type_e::change_link_flags:
			{
			    if (nodes_itr == nodes.end())
			    {
				    throw std::exception("state does not exist on stack, cannot change link flags");
			    }

			    //modify link flags of state on stack
                nodes_itr->link_flags = operation.link_flags;

                //clear changing link flags flag from node flags
                nodes_itr->flags &= ~state_flag_changing_link_flags;
			}
				break;
			}

			operations.pop();
		}

		//call on_enter on pushed states
		for (auto& state : pushed_states)
		{
			state->on_enter();
        }

        //call on_exit on popped states
        for (auto& state : popped_states)
        {
            state->on_exit();
        }

		//check if stack was modified
		if (did_nodes_change)
		{
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

			//compare previous node flags to current ones, call start/stop flag events to states if flags changed
			state_flags_type flags = state_flag_all;

			for (auto nodes_reverse_itr = nodes.rbegin(); nodes_reverse_itr != nodes.rend(); ++nodes_reverse_itr)
			{
				const auto previous_node_flags = nodes_reverse_itr->flags;

				//render
				if ((previous_node_flags & state_flag_render) == state_flag_none && (flags & state_flag_render) == state_flag_render)
				{
					nodes_reverse_itr->state->on_start_render();
				}
				else if ((previous_node_flags & state_flag_render) == state_flag_render && (flags & state_flag_render) == state_flag_none)
				{
					nodes_reverse_itr->state->on_stop_render();
				}

				//input
				if ((previous_node_flags & state_flag_input) == state_flag_none && (flags & state_flag_input) == state_flag_input)
				{
					nodes_reverse_itr->state->on_start_input();
				}
				else if ((previous_node_flags & state_flag_input) == state_flag_input && (flags & state_flag_input) == state_flag_none)
				{
					nodes_reverse_itr->state->on_stop_input();
				}

				//tick
				if ((previous_node_flags & state_flag_tick) == state_flag_none && (flags & state_flag_tick) == state_flag_tick)
				{
					nodes_reverse_itr->state->on_start_tick();
				}
				else if ((previous_node_flags & state_flag_tick) == state_flag_tick && (flags & state_flag_tick) == state_flag_none)
				{
					nodes_reverse_itr->state->on_stop_tick();
				}

				nodes_reverse_itr->flags = flags;

				flags &= nodes_reverse_itr->link_flags;
			}
		}

		//tick states
		for (auto nodes_reverse_itr = nodes.rbegin(); nodes_reverse_itr != nodes.rend(); ++nodes_reverse_itr)
		{
			if ((nodes_reverse_itr->flags & state_flag_tick) == state_flag_none)
			{
				//state not ticking, break
				break;
			}

			nodes_reverse_itr->state->tick(dt);
		}
	}

	void state_mgr_t::render()
	{
		for (auto& node : nodes)
		{
			if ((node.flags & state_flag_render) == state_flag_render)
			{
				node.state->render();
			}
		}
	}

	void state_mgr_t::on_input_event(input_event_t& input_event)
	{
		for (auto nodes_reverse_itr = nodes.rbegin(); nodes_reverse_itr != nodes.rend(); ++nodes_reverse_itr)
		{
            if ((nodes_reverse_itr->flags & state_flag_input) != state_flag_input ||
                (nodes_reverse_itr->flags & state_flag_popping) == state_flag_popping)
            {
                //state not handling input or is being popped, return
                return;
            }

			nodes_reverse_itr->state->on_input_event(input_event);

			if (input_event.is_consumed)
			{
				//state consumed input event, return
				return;
			}
		}
	}

#if defined(_WIN32) || defined(WIN32)
    void state_mgr_t::on_window_event(window_event_t& window_event)
    {
        for (auto& node : nodes)
        {
            node.state->on_window_event(window_event);
        }
    }
#endif

	//push a state onto the stack
	void state_mgr_t::push(const state_type& state, state_flags_type link_flags)
	{
		if (state == nullptr)
		{
			throw std::exception();
		}

        operation_t operation;
        operation.type = operation_t::type_e::push;
        operation.state = state;
        operation.link_flags = link_flags;

		operations.push(operation);
	}

	//pop a specific state off of the stack
	void state_mgr_t::pop(const state_type& state)
    {
        if (state == nullptr)
        {
            throw std::invalid_argument("");
        }

        const auto nodes_itr = std::find_if(nodes.begin(), nodes.end(), [&](const node_t& node)
        {
            return node.state == state;
        });

        if (nodes_itr == nodes.end())
        {
            //state does not exist on stack
            throw std::out_of_range("");
        }

		operation_t operation;
		operation.type = operation_t::type_e::pop;
		operation.state = state;

		operations.push(operation);

        //add popping flag to node flags
        nodes_itr->flags |= state_flag_popping;
	}

	void state_mgr_t::change_link_flags(const state_type& state, state_flags_type link_flags)
	{
		if (state == nullptr)
		{
			throw std::exception();
        }

        const auto nodes_itr = std::find_if(nodes.begin(), nodes.end(), [&](const node_t& node)
        {
            return node.state == state;
        });

        if (nodes_itr == nodes.end())
        {
            //state does not exist on stack
            throw std::out_of_range("");
        }

		operation_t operation;
		operation.type = operation_t::type_e::change_link_flags;
		operation.state = state;
		operation.link_flags = link_flags;

        operations.push(operation);

        //add changing link flags flag to node flags
        nodes_itr->flags |= state_flag_changing_link_flags;
	}

	void state_mgr_t::purge()
	{
		nodes.clear();
	}

	size_t state_mgr_t::count() const
	{
		return nodes.size();
	}

	state_mgr_t::state_type state_mgr_t::at(size_t index) const
	{
		if (index >= nodes.size())
		{
			throw std::out_of_range("");
		}

		auto nodes_reverse_itr = nodes.rbegin();

		while (index > 0)
		{
			++nodes_reverse_itr;

			--index;
		}

		return nodes_reverse_itr->state;
	}

	size_t state_mgr_t::index_of(const state_type& state) const
	{
		const auto nodes_itr = std::find_if(nodes.begin(), nodes.end(), [&](const node_t& node)
		{
			return node.state == state;
		});

		return std::distance(nodes_itr, nodes.end()) - 1;
	}

	state_flags_type state_mgr_t::get_link_flags(const state_type& state) const
	{
		const auto nodes_itr = std::find_if(nodes.begin(), nodes.end(), [&](const node_t& node)
		{
			return node.state == state;
		});

		if (nodes_itr == nodes.end())
		{
			throw std::out_of_range("");
		}

		return nodes_itr->link_flags;
	}

	state_flags_type state_mgr_t::get_flags(const state_type& state) const
	{
		const auto nodes_itr = std::find_if(nodes.begin(), nodes.end(), [&](const node_t& node)
		{
			return node.state == state;
		});

		if (nodes_itr == nodes.end())
		{
			throw std::out_of_range("");
		}

		return nodes_itr->flags;
	}

	bool state_mgr_t::is_state_rendering(const state_type& state) const
	{
		return (get_flags(state) & state_flag_render) == state_flag_render;
	}

	bool state_mgr_t::is_state_ticking(const state_type& state) const
	{
		return (get_flags(state) & state_flag_tick) == state_flag_tick;
	}

	bool state_mgr_t::is_state_handling_input(const state_type& state) const
	{
		return (get_flags(state) & state_flag_input) == state_flag_input;
	}

    bool state_mgr_t::is_state_popping(const state_type& state) const
    {
        return (get_flags(state) & state_flag_popping) == state_flag_popping;
    }

    bool state_mgr_t::is_state_changing_link_flags(const state_type& state) const
    {
        return (get_flags(state) & state_flag_changing_link_flags) == state_flag_changing_link_flags;
    }
}
