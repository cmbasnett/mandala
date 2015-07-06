//std
#include <sstream>

//mandala
#include "state_mgr.hpp"
#include "state.hpp"

namespace mandala
{
	state_mgr_t states;

	void state_mgr_t::tick(float32_t dt)
	{
		bool did_nodes_change = !operations.empty();

		state_type previous_top_state;

		if (!operations.empty())
		{
			previous_top_state = nodes.empty() ? nullptr : nodes.rbegin()->state;
		}

		std::vector<state_type> pushed_states;
		std::vector<std::pair<state_type, mandala::state_flags_type>> popped_states;

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
			case operation_t::type_e::POP:
			{
			    if (nodes_itr == nodes.end())
			    {
				    throw std::exception("state does not exist on stack, cannot pop");
			    }

			    popped_states.push_back(std::make_pair(nodes_itr->state, nodes_itr->flags));

			    //remove state from stack
                nodes.erase(nodes_itr);
			}
				break;
			case operation_t::type_e::PUSH:
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
			case operation_t::type_e::CHANGE_LINK_FLAGS:
			{
			    if (nodes_itr == nodes.end())
			    {
				    throw std::exception("state does not exist on stack, cannot change link flags");
			    }

			    //modify link flags of state on stack
                nodes_itr->link_flags = operation.link_flags;

                //clear changing link flags flag from node flags
                nodes_itr->flags &= ~STATE_FLAG_CHANGING_LINK_FLAGS;
			}
				break;
            case operation_t::type_e::PURGE:
                while (!nodes.empty())
                {
                    popped_states.push_back(std::make_pair(nodes.back().state, nodes.back().flags));

                    nodes.pop_back();
                }

                break;
            }

            operations.pop_front();
		}

		//call on_enter on pushed states
		for (auto& state : pushed_states)
		{
			state->on_enter();
        }

		//check if stack was modified
		if (did_nodes_change)
		{
			//check if top state changed
			if (!nodes.empty() && previous_top_state != nodes.rbegin()->state)
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
			state_flags_type flags = STATE_FLAG_ALL;

			for (auto nodes_reverse_itr = nodes.rbegin(); nodes_reverse_itr != nodes.rend(); ++nodes_reverse_itr)
			{
				const auto previous_node_flags = nodes_reverse_itr->flags;

				//render
				if ((previous_node_flags & STATE_FLAG_RENDER) == STATE_FLAG_NONE && (flags & STATE_FLAG_RENDER) == STATE_FLAG_RENDER)
				{
					nodes_reverse_itr->state->on_start_render();
				}
				else if ((previous_node_flags & STATE_FLAG_RENDER) == STATE_FLAG_RENDER && (flags & STATE_FLAG_RENDER) == STATE_FLAG_NONE)
				{
					nodes_reverse_itr->state->on_stop_render();
				}

				//input
				if ((previous_node_flags & STATE_FLAG_INPUT) == STATE_FLAG_NONE && (flags & STATE_FLAG_INPUT) == STATE_FLAG_INPUT)
				{
					nodes_reverse_itr->state->on_start_input();
				}
				else if ((previous_node_flags & STATE_FLAG_INPUT) == STATE_FLAG_INPUT && (flags & STATE_FLAG_INPUT) == STATE_FLAG_NONE)
				{
					nodes_reverse_itr->state->on_stop_input();
				}

				//tick
				if ((previous_node_flags & STATE_FLAG_TICK) == STATE_FLAG_NONE && (flags & STATE_FLAG_TICK) == STATE_FLAG_TICK)
				{
					nodes_reverse_itr->state->on_start_tick();
				}
				else if ((previous_node_flags & STATE_FLAG_TICK) == STATE_FLAG_TICK && (flags & STATE_FLAG_TICK) == STATE_FLAG_NONE)
				{
					nodes_reverse_itr->state->on_stop_tick();
				}

				nodes_reverse_itr->flags = flags;

				flags &= nodes_reverse_itr->link_flags;
			}
		}

        //call on_exit on popped states
        for (auto& state : popped_states)
        {
            if ((state.second & STATE_FLAG_RENDER) == STATE_FLAG_RENDER)
            {
                state.first->on_stop_render();
            }

            if ((state.second & STATE_FLAG_INPUT) == STATE_FLAG_INPUT)
            {
                state.first->on_stop_input();
            }

            if ((state.second & STATE_FLAG_TICK) == STATE_FLAG_TICK)
            {
                state.first->on_stop_tick();
            }

            state.first->on_exit();
        }

		//tick states
		for (auto nodes_reverse_itr = nodes.rbegin(); nodes_reverse_itr != nodes.rend(); ++nodes_reverse_itr)
		{
			if ((nodes_reverse_itr->flags & STATE_FLAG_TICK) == STATE_FLAG_NONE)
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
			if ((node.flags & STATE_FLAG_RENDER) == STATE_FLAG_RENDER)
			{
				node.state->render();
			}
		}
	}

	bool state_mgr_t::on_input_event(input_event_t& input_event)
	{
		for (auto nodes_itr = nodes.rbegin(); nodes_itr != nodes.rend(); ++nodes_itr)
		{
            if ((nodes_itr->flags & STATE_FLAG_INPUT) != STATE_FLAG_INPUT ||
                (nodes_itr->flags & STATE_FLAG_POPPING) == STATE_FLAG_POPPING)
            {
                //state not handling input or is being popped, return
                return false;
            }

            if (nodes_itr->state->on_input_event(input_event))
            {
                return true;
            }
		}

        return false;
	}

#if defined(MANDALA_PC)
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
			throw std::invalid_argument("state cannot be null");
		}

        operation_t operation;
        operation.type = operation_t::type_e::PUSH;
        operation.state = state;
        operation.link_flags = link_flags;

		operations.push_back(operation);
	}

	//pop a specific state off of the stack
	void state_mgr_t::pop(const state_type& state)
    {
        if (state == nullptr)
        {
            throw std::invalid_argument("state cannot be null");
        }

        const auto nodes_itr = std::find_if(nodes.begin(), nodes.end(), [&](const node_t& node)
        {
            return node.state == state;
        });

        if (nodes_itr == nodes.end())
        {
            //state does not exist on the stack
            //check operation queue to see if state is about to be pushed
            const auto operations_itr = std::find_if(operations.begin(), operations.end(), [&](const operation_t& operation)
            {
                return operation.state == state && operation.type == operation_t::type_e::PUSH;
            });

            if (operations_itr != operations.end())
            {
                //state is in operation queue to be pushed, erase that operation (effectively a no-op)
                operations.erase(operations_itr);

                return;
            }
            else
            {
                throw std::exception("state is not on the stack nor in queue to be pushed");
            }
        }

        //add popping flag to node flags
        nodes_itr->flags |= STATE_FLAG_POPPING;

		operation_t operation;
		operation.type = operation_t::type_e::POP;
		operation.state = state;

		operations.push_back(operation);
	}

	void state_mgr_t::change_link_flags(const state_type& state, state_flags_type link_flags)
	{
		if (state == nullptr)
		{
            throw std::invalid_argument("state cannot be null");
        }

        const auto nodes_itr = std::find_if(nodes.begin(), nodes.end(), [&](const node_t& node)
        {
            return node.state == state;
        });

        if (nodes_itr == nodes.end())
        {
            //state does not exist on the stack
            //check operation queue to see if state is about to be pushed
            const auto operations_itr = std::find_if(operations.begin(), operations.end(), [&](const operation_t& operation)
            {
                return operation.state == state && operation.type == operation_t::type_e::PUSH;
            });

            if (operations_itr != operations.end())
            {
                //state is in operation queue to be pushed, change operations' link flags
                operations_itr->link_flags = link_flags;

                return;
            }
            else
            {
                throw std::exception("state is not on the stack nor in queue to be pushed");
            }
        }

		operation_t operation;
		operation.type = operation_t::type_e::CHANGE_LINK_FLAGS;
		operation.state = state;
		operation.link_flags = link_flags;

        operations.push_back(operation);

        //add changing link flags flag to node flags
        nodes_itr->flags |= STATE_FLAG_CHANGING_LINK_FLAGS;
	}

	void state_mgr_t::purge()
	{
        operation_t operation;
        operation.type = operation_t::type_e::PURGE;

        operations.push_back(operation);
	}

	size_t state_mgr_t::count() const
	{
		return nodes.size();
	}

	state_mgr_t::state_type state_mgr_t::at(size_t index) const
	{
		if (index >= nodes.size())
		{
            std::ostringstream oss;
            oss << "no state at index " << index;

			throw std::out_of_range(oss.str());
		}

        auto nodes_reverse_itr = nodes.rbegin();

		while (index-- > 0)
		{
			++nodes_reverse_itr;
		}

		return nodes_reverse_itr->state;
	}

	boost::optional<size_t> state_mgr_t::index_of(const state_type& state) const
	{
        boost::optional<size_t> index;

		const auto nodes_reverse_itr = std::find_if(nodes.rbegin(), nodes.rend(), [&](const node_t& node)
		{
			return node.state == state;
		});

        if (nodes_reverse_itr != nodes.rend())
        {
            index = std::distance(nodes.rbegin(), nodes_reverse_itr);
        }

        return index;
	}

	state_flags_type state_mgr_t::get_link_flags(const state_type& state) const
	{
		const auto nodes_itr = std::find_if(nodes.begin(), nodes.end(), [&](const node_t& node)
		{
			return node.state == state;
		});

		if (nodes_itr == nodes.end())
		{
			throw std::out_of_range("state is not on the stack");
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
			throw std::out_of_range("state is not on the stack");
		}

		return nodes_itr->flags;
	}

	bool state_mgr_t::is_state_rendering(const state_type& state) const
	{
		return (get_flags(state) & STATE_FLAG_RENDER) == STATE_FLAG_RENDER;
	}

	bool state_mgr_t::is_state_ticking(const state_type& state) const
	{
		return (get_flags(state) & STATE_FLAG_TICK) == STATE_FLAG_TICK;
	}

	bool state_mgr_t::is_state_handling_input(const state_type& state) const
	{
		return (get_flags(state) & STATE_FLAG_INPUT) == STATE_FLAG_INPUT;
	}

    bool state_mgr_t::is_state_popping(const state_type& state) const
    {
        return (get_flags(state) & STATE_FLAG_POPPING) == STATE_FLAG_POPPING;
    }

    bool state_mgr_t::is_state_changing_link_flags(const state_type& state) const
    {
        return (get_flags(state) & STATE_FLAG_CHANGING_LINK_FLAGS) == STATE_FLAG_CHANGING_LINK_FLAGS;
    }
}