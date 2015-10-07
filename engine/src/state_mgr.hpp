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
    struct app;
    struct state;

#if defined(MANDALA_PC)
    struct window_event;
#endif

    typedef u8 state_flags_type;

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

    struct state_mgr
    {
        typedef boost::shared_ptr<state> state_type;

        struct node_t
        {
            state_type state;
            state_flags_type link_flags = STATE_FLAG_NONE;
            state_flags_type flags = STATE_FLAG_NONE;
            i32 weight = 0;
        };

        struct operation_t
        {
            enum class type_e : u8
            {
                PUSH,
                POP,
                CHANGE_LINK_FLAGS,
                CHANGE_WEIGHT,
                PURGE
            };

            type_e type = type_e::PUSH;
            state_type state;
            state_flags_type link_flags = STATE_FLAG_NONE;
            size_t index = 0;
            i32 weight = 0;
        };

        state_mgr() = default;

        void tick(f32 dt);
        void render();
        bool on_input_event(input_event_t& input_event);
#if defined(MANDALA_PC)
        void on_window_event(window_event& window_event);
#endif

        void push(const state_type& state, state_flags_type link_flags, i32 weight = 0);
        void pop(const state_type& state);
        void change_link_flags(const state_type& state, state_flags_type link_flags);
        void change_weight(const state_type& state, i32 weight);
        void purge();

        state_flags_type get_flags(const state_type& state) const;
        state_flags_type get_link_flags(const state_type& state) const;
        i32 get_weight(const state_type& state) const;
        size_t count() const;
        state_type at(size_t node_index) const;
        boost::optional<size_t> index_of(const state_type& state) const;
        bool is_state_rendering(const state_type& state) const;
        bool is_state_ticking(const state_type& state) const;
        bool is_state_handling_input(const state_type& state) const;
        bool is_state_popping(const state_type& state) const;
        bool is_state_changing_link_flags(const state_type& state) const;
        
    private:
        state_mgr(const state_mgr&) = delete;
        state_mgr& operator=(const state_mgr&) = delete;

        std::list<node_t> nodes;
        std::deque<operation_t> operations;
    };

    extern state_mgr states;
}