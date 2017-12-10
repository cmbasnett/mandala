// std
#include <string>

// naga
#include "types.hpp"

struct Action
{
	Action(const std::string& name);

	const std::string& get_name() const { return name; }

private:
	std::string name;
};
