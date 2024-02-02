#pragma once

#include "Xuzumi/Instrumentation/PerformanceProfiler.hpp"
#include <chrono>

namespace Xuzumi
{
	class PerformanceCapture
	{
	public:
		PerformanceCapture(
			const PerformanceProfiler& profiler,
			std::string_view captureName
		);
		~PerformanceCapture();

		void Capture();

	private:
		const PerformanceProfiler* mProfiler = nullptr;
		std::string_view mCaptureName;

		std::chrono::high_resolution_clock::time_point mStart;
	};
}

