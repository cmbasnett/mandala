#pragma once

//mandala
#include "input_event.hpp"

//std
#include <vector>
#include <deque>
#include <list>

//boost
#include <boost\optional.hpp>
#include <boost\shared_ptr.hpp>

namespace mandala
{
	struct app_t;
	struct state_t;

#if defined(MANDALA_PC)
    struct window_event_t;
#endif

	typedef uint8_t state_flags_type;

	enum : state_flags_type
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

	struct state_mgr_t
	{
		typedef boost::shared_ptr<state_t> state_type;

		struct node_t
		{
			state_type state;
			state_flags_type link_flags = STATE_FLAG_NONE;
			state_flags_type flags = STATE_FLAG_NONE;
		};

		struct operation_t
		{
			enum class type_e : uint8_t
			{
				PUSH,
				POP,
				CHANGE_LINK_FLAGS,
                PURGE
			};

            type_e type = type_e::PUSH;
			state_type state;
			state_flags_type link_flags = STATE_FLAG_NONE;
			size_t index = 0;
		};

		state_mgr_t() = default;

		void tick(float32_t dt);
		void render();
		void on_input_event(input_event_t& input_event);
#if defined(MANDALA_PC)
        void on_window_event(window_event_t& window_event);
#endif

		void push(const state_type& state, state_flags_type link_flags);
		void pop(const state_type& state);
		void change_link_flags(const state_type& state, state_flags_type link_flags);
		void purge();

        state_flags_type get_flags(const state_type& state) const;
        state_flags_type get_link_flags(const state_type& state) const;
        size_t count() const;
        state_type at(size_t node_index) const;
        boost::optional<size_t> index_of(const state_type& state) const;
        bool is_state_rendering(const state_type& state) const;
        bool is_state_ticking(const state_type& state) const;
        bool is_state_handling_input(const state_type& state) const;
        bool is_state_popping(const state_type& state) const;
        bool is_state_changing_link_flags(const state_type& state) const;
		
	private:
		state_mgr_t(const state_mgr_t&) = delete;
		state_mgr_t& operator=(const state_mgr_t&) = delete;

		std::list<node_t> nodes;
		std::deque<operation_t> operations;
	};

	extern state_mgr_t states;
}