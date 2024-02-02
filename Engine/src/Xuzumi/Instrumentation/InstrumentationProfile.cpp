#include "Xuzumi/Instrumentation/InstrumentationProfile.hpp"

namespace Xuzumi
{
	const Logger& DefaultInstrumentationProfile::GetCoreLogger() const
	{
		return mCoreLogger;
	}

	const Logger& DefaultInstrumentationProfile::GetAppLogger() const
	{
		return mAppLogger;
	}

	const AssertService&
	DefaultInstrumentationProfile::GetCoreAssertService() const
	{
		return mCoreAssertService;
	}

	const AssertService&
	DefaultInstrumentationProfile::GetAppAssertService() const
	{
		return mAppAssertService;
	}

	const PerformanceProfiler&
	DefaultInstrumentationProfile::GetPerformanceProfiler() const
	{
		return mPerformanceProfiler;
	}
}

