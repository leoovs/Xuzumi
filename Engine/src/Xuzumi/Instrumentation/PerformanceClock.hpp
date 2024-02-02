#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi::Internal
{
	class PerformanceClock
	{
	public:
		static std::chrono::high_resolution_clock::time_point UniqueNow();

	private:
		static std::chrono::high_resolution_clock::time_point sUniqueNow;
	};
}

