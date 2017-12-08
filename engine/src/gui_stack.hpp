// naga
#include "gui_node.hpp"

namespace naga
{
	struct GuiStack : GUINode
	{
		enum class Direction
		{
			HORIZONTAL,
			VERTICAL
		};

		Direction get_direction() const { return direction; }
		void set_direction(Direction direction) { this->direction = direction; dirty(); }

	private:
		Direction direction;
	};
}