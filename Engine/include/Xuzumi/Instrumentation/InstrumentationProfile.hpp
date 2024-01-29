#pragma once

#include "Xuzumi/Instrumentation/Logger.hpp"
#include "Xuzumi/Instrumentation/AssertService.hpp"

namespace Xuzumi
{
	// TODO: turn factory into state, `Create*` into `Get*`: profile itself should
	// own loggers and assert handlers. The `InstrumentationSession` should then
	// own `InstrumentationProfile` instance.
	class InstrumentationProfile
	{
	public:
		virtual ~InstrumentationProfile() = default;

		virtual Logger CreateAppLogger() const = 0;
		virtual Logger CreateCoreLogger() const = 0;
		virtual AssertHandler CreateAppAssertHandler() const = 0;
		virtual AssertHandler CreateCoreAssertHandler() const = 0;
	};
}

