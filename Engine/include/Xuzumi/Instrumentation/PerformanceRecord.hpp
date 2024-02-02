#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	struct PerformanceRecord
	{
		std::string BlockName;

		std::chrono::nanoseconds StartTimestamp;
		std::chrono::nanoseconds Duration;
		std::thread::id ThreadID;
	};
}

