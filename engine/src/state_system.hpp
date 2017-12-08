#pragma once

//naga
#include "input_event.hpp"

//std
#include <vector>
#include <deque>
#include <list>

//boost
#include <boost\optional.hpp>
#include <boost\shared_ptr.hpp>

namespace naga
{
    struct State;

#if defined(NAGA_PC)
    struct WindowEvent;
#endif

    typedef u8 StateFlagsType;

	enum : StateFlagsType
    {
        STATE_FLAG_NONE = (0 << 0),
        STATE_FLAG_RENDER = (1 << 0),
        STATE_FLAG_INPUT = (1 << 1),
        STATE_FLAG_TICK = (1 << 2),
        STATE_FLAG_POPPING = (1 << 3),
        STATE_FLAG_CHANGING_LINK_FLAGS = (1 << 4),
        STATE_FLAG_RENDER_INPUT = (STATE_FLAG_RENDER | STATE_FLAG_INPUT),
        STATE_FLAG_RENDER_TICK = (STATE_FLAG_RENDER | STATE_FLAG_TICK),
        STATE_FLAG_INPUT_TICK = (STATE_FLAG_INPUT | STATE_FLAG_TICK),
        STATE_FLAG_ALL = (STATE_FLAG_RENDER | STATE_FLAG_INPUT | STATE_FLAG_TICK)
    };

    struct StateSystem
    {
		typedef boost::shared_ptr<State> StateType;
		typedef i32 WeightType;

        struct Node
        {
			StateType state;
			StateFlagsType link_flags = STATE_FLAG_NONE;
			StateFlagsType flags = STATE_FLAG_NONE;
			WeightType weight = 0;
        };

        struct Operation
        {
            enum class Type : u8
            {
                PUSH,
                POP,
                CHANGE_LINK_FLAGS,
                CHANGE_WEIGHT,
                PURGE
            };

			Type type = Type::PUSH;
			StateType state;
			StateFlagsType link_flags = STATE_FLAG_NONE;
            size_t index = 0;
			WeightType weight = 0;
        };

		StateSystem() = default;

        void tick(f32 dt);
        void render();
        bool on_input_event(InputEvent& input_event);
#if defined(NAGA_PC)
        void on_window_event(WindowEvent& window_event);
#endif

		void push(const StateType& state, StateFlagsType link_flags, WeightType weight = 0);
		void pop(const StateType& state);
		void change_link_flags(const StateType& state, StateFlagsType link_flags);
		void change_weight(const StateType& state, WeightType weight);
        void purge();

		StateFlagsType get_flags(const StateType& state) const;
		StateFlagsType get_link_flags(const StateType& state) const;
		WeightType get_weight(const StateType& state) const;
        size_t count() const;
		StateType at(size_t node_index) const;
		bool contains(const StateType& state) const;
		boost::optional<size_t> index_of(const StateType& state) const;
		bool is_state_rendering(const StateType& state) const;
		bool is_state_ticking(const StateType& state) const;
		bool is_state_handling_input(const StateType& state) const;
		bool is_state_popping(const StateType& state) const;
		bool is_state_changing_link_flags(const StateType& state) const;
        
    private:
		StateSystem(const StateSystem&) = delete;
		StateSystem& operator=(const StateSystem&) = delete;

        std::list<Node> nodes;
        std::deque<Operation> operations;
    };

    extern StateSystem states;
}