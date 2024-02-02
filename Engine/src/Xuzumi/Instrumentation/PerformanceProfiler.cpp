#include "Xuzumi/Instrumentation/PerformanceProfiler.hpp"

namespace Xuzumi
{
	PerformanceProfiler::PerformanceProfiler(
		std::unique_ptr<PerformanceRecordStream> outputStream
	)
		: mOutputStream(std::move(outputStream))
	{}

	void PerformanceProfiler::OnCapture(const PerformanceRecord& record) const
	{
		if (mOutputStream)
		{
			mOutputStream->Out(record);
		}
	}
}

