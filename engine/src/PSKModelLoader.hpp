#include "psk.hpp"
#include "model_loader.h"

namespace naga
{
	struct PSKModelLoader : ModelLoader
	{
		virtual void load(std::istream& istream);
	};
}