#include "Xuzumi/Instrumentation/PerformanceCapture.hpp"

#include "Xuzumi/Instrumentation/PerformanceClock.hpp"

namespace Xuzumi
{
	PerformanceCapture::PerformanceCapture(
		const PerformanceProfiler& profiler,
		std::string_view captureName
	)
		: mProfiler(&profiler)
		, mCaptureName(captureName)
		, mStart(Internal::PerformanceClock::UniqueNow())
	{}

	PerformanceCapture::~PerformanceCapture()
	{
		Capture();
	}

	void PerformanceCapture::Capture()
	{
		if (nullptr == mProfiler)
		{
			return;
		}

		auto end = Internal::PerformanceClock::UniqueNow();

		PerformanceRecord record;
		record.StartTimestamp = std::chrono::duration_cast<
			std::chrono::nanoseconds
		>(mStart.time_since_epoch());
		record.Duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
			end - mStart
		);
		record.ThreadID = std::this_thread::get_id();
		record.BlockName = mCaptureName;

		mProfiler->OnCapture(record);
	}
}

