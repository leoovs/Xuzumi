#pragma once

#include "Xuzumi/Instrumentation/PerformanceRecordStream.hpp"

namespace Xuzumi
{
	class PerformanceProfiler
	{
	public:
		PerformanceProfiler(std::unique_ptr<PerformanceRecordStream> outputStream);

		void OnCapture(const PerformanceRecord& record) const;

	private:
		std::unique_ptr<PerformanceRecordStream> mOutputStream;
	};
}

