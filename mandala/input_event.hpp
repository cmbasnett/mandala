#pragma once

//mandala
#include "types.hpp"

namespace mandala
{
	struct input_event_t
	{
		typedef size_t id_type;
		typedef uint8_t device_index_type;
		typedef uint8_t mods_type;

		enum mods_t : mods_type
		{
			mod_shift = (1 << 0),
			mod_ctrl = (1 << 1),
			mod_alt = (1 << 2),
			mod_super = (1 << 3)
		};

		enum class device_type_t : int8_t
		{
			none = -1,
			touch,
			keyboard,
			gamepad,
			count
		};

		struct touch_t
		{
			typedef size_t touch_id_type;
			typedef vec2_f64_t position_type;

			enum class button_t : int8_t
			{
				none = -1,
				left,
				right,
				middle,
				four,
				five,
				six,
				seven,
				eight,
				count,
			};

			enum class type_t : int8_t
			{
				none = -1,
				button_press,
				button_release,
				scroll,
				move
			};

			touch_id_type id = 0;
			type_t type = type_t::none;
			button_t button = button_t::none;
			mods_type mods = 0;
			position_type position;
			position_type position_delta;
		};

		struct keyboard_t
		{
			enum class type_t : int8_t
			{
				none = -1,
				key_release,
				key_press,
				key_repeat,
				character
			};

			enum class key_t : int16_t
			{
				none			   = -1,
				space              = 32,
				apostrophe         = 39,  /* ' */
				comma              = 44,  /* , */
				minus              = 45,  /* - */
				period             = 46,  /* . */
				slash              = 47,  /* / */
				nr_0               = 48,
				nr_1               = 49,
				nr_2               = 50,
				nr_3               = 51,
				nr_4               = 52,
				nr_5               = 53,
				nr_6               = 54,
				nr_7               = 55,
				nr_8               = 56,
				nr_9               = 57,
				semicolon          = 59,  /* ; */
				equal              = 61,  /* =  */
				a                  = 65,
				b                  = 66,
				c                  = 67,
				d                  = 68,
				e                  = 69,
				f                  = 70,
				g                  = 71,
				h                  = 72,
				i                  = 73,
				j                  = 74,
				k                  = 75,
				l                  = 76,
				m                  = 77,
				n                  = 78,
				o                  = 79,
				p                  = 80,
				q                  = 81,
				r                  = 82,
				s                  = 83,
				t                  = 84,
				u                  = 85,
				v                  = 86,
				w                  = 87,
				x                  = 88,
				y                  = 89,
				z                  = 90,
				left_bracket       = 91,  /* [ */
				backslash          = 92,  /* \ */
				right_bracket      = 93,  /* ] */
				grave_accent       = 96,  /* ` */
				world_1            = 161, /* non-us #1 */
				world_2            = 162, /* non-us #2 */
				escape             = 256,
				enter              = 257,
				tab                = 258,
				backspace          = 259,
				insert             = 260,
				del                = 261,
				right              = 262,
				left               = 263,
				down               = 264,
				up                 = 265,
				page_up            = 266,
				page_down          = 267,
				home               = 268,
				end                = 269,
				caps_lock          = 280,
				scroll_lock        = 281,
				num_lock           = 282,
				print_screen       = 283,
				pause              = 284,
				f1                 = 290,
				f2                 = 291,
				f3                 = 292,
				f4                 = 293,
				f5                 = 294,
				f6                 = 295,
				f7                 = 296,
				f8                 = 297,
				f9                 = 298,
				f10                = 299,
				f11                = 300,
				f12                = 301,
				f13                = 302,
				f14                = 303,
				f15                = 304,
				f16                = 305,
				f17                = 306,
				f18                = 307,
				f19                = 308,
				f20                = 309,
				f21                = 310,
				f22                = 311,
				f23                = 312,
				f24                = 313,
				f25                = 314,
				kp_0               = 320,
				kp_1               = 321,
				kp_2               = 322,
				kp_3               = 323,
				kp_4               = 324,
				kp_5               = 325,
				kp_6               = 326,
				kp_7               = 327,
				kp_8               = 328,
				kp_9               = 329,
				kp_decimal         = 330,
				kp_divide          = 331,
				kp_multiply        = 332,
				kp_subtract        = 333,
				kp_add             = 334,
				kp_enter           = 335,
				kp_equal           = 336,
				left_shift         = 340,
				left_control       = 341,
				left_alt           = 342,
				left_super         = 343,
				right_shift        = 344,
				right_control      = 345,
				right_alt          = 346,
				right_super        = 347,
				menu               = 348
			};

			type_t type = type_t::none;
			key_t key = key_t::none;
			mods_type mods = 0;
			wchar_t character = L'\0';
		};

		struct gamepad_t
		{
			typedef uint8_t index_type;
			typedef float32_t axis_value_type;

			enum class type_t : int8_t
			{
				none = -1,
				button_press,
				button_release,
				axis_move
			};

            index_type button_index = 0;
            index_type axis_index = 0;
            axis_value_type axis_value = 0;
            axis_value_type axis_value_delta = 0;
			type_t type = type_t::none;
		};

		id_type id = 0;
		device_type_t device_type = device_type_t::none;
		device_index_type device_index = 0;
		touch_t touch;
		keyboard_t keyboard;
		gamepad_t gamepad;
		bool is_consumed = false;
	};
};
