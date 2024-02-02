#include "Xuzumi/Instrumentation/PerformanceClock.hpp"
#include <chrono>

namespace Xuzumi::Internal
{
		std::chrono::high_resolution_clock::time_point
		PerformanceClock::UniqueNow()
		{
			// HACK: not really the most elegant way to ensure that `now` always
			// outputs different `time_point`s. Perhaps switch to something
			// platform-specific and more precise, like `QueryPerformanceCounter` on
			// Windows.

			auto now = sUniqueNow;
			for (; now == sUniqueNow; now = std::chrono::high_resolution_clock::now());

			return sUniqueNow = now;
		}

		std::chrono::high_resolution_clock::time_point PerformanceClock::sUniqueNow;
}

