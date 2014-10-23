#pragma once

//mandala
#include "state.h"

namespace mandala
{
	struct input_event_t;

	namespace armada
	{
		struct performance_state_t : state_t
		{
			performance_state_t();

			virtual void render() override;
		};
	};
};
