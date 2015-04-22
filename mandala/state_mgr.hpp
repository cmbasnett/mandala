#pragma once

//mandala
#include "input_event.hpp"

//std
#include <vector>
#include <memory>
#include <queue>
#include <list>

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
		state_flag_none = (0 << 0),
		state_flag_render = (1 << 0),
		state_flag_input = (1 << 1),
		state_flag_tick = (1 << 2),
        state_flag_popping = (1 << 3),
        state_flag_changing_link_flags = (1 << 4),
		state_flag_render_input = (state_flag_render | state_flag_input),
		state_flag_render_tick = (state_flag_render | state_flag_tick),
		state_flag_input_tick = (state_flag_input | state_flag_tick),
		state_flag_all = (state_flag_render | state_flag_input | state_flag_tick)
	};

	struct state_mgr_t
	{
		typedef std::shared_ptr<state_t> state_type;

		struct node_t
		{
			state_type state;
			state_flags_type link_flags = state_flag_none;
			state_flags_type flags = state_flag_none;
		};

		struct operation_t
		{
			enum class type_e : uint8_t
			{
				push,
				pop,
				change_link_flags,
                purge
			};

            type_e type = type_e::push;
			state_type state;
			state_flags_type link_flags = state_flag_none;
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
        size_t index_of(const state_type& state) const;
        bool is_state_rendering(const state_type& state) const;
        bool is_state_ticking(const state_type& state) const;
        bool is_state_handling_input(const state_type& state) const;
        bool is_state_popping(const state_type& state) const;
        bool is_state_changing_link_flags(const state_type& state) const;
		
	private:
		state_mgr_t(const state_mgr_t&) = delete;
		state_mgr_t& operator=(const state_mgr_t&) = delete;

		std::list<node_t> nodes;
		std::queue<operation_t> operations;
	};

	extern state_mgr_t states;
}