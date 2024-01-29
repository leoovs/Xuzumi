#pragma once

#include "Xuzumi/Instrumentation/Logger.hpp"

namespace Xuzumi
{
	class InstrumentationProfile
	{
	public:
		virtual ~InstrumentationProfile() = default;

		virtual Logger CreateAppLogger() const = 0;
		virtual Logger CreateCoreLogger() const = 0;
	};
}

