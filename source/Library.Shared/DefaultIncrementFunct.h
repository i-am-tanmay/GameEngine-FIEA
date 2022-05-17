#pragma once
#include <cstdint>
#include <stdexcept>
#include <functional>

namespace FIEAGameEngine
{
	struct DefaultIncrementFunct final
	{
		std::size_t operator()(std::size_t, std::size_t capacity) const
		{
			if (capacity > 0u) return capacity * 2u;
			else return 1u;
		}
	};
}