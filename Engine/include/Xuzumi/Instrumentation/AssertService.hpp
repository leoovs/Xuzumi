#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Instrumentation/Logger.hpp"
#include "Xuzumi/Instrumentation/AssertRecord.hpp"

namespace Xuzumi
{
	using AssertHandler = std::function<void(const AssertRecord& record)>;

	class AssertService
	{
	public:
		static AssertHandler CreateDefaultHandler();
		static AssertHandler CreateHandlerFromLogger(const Logger& logger);

		AssertService() = default;

		AssertService(const AssertService& other);

		explicit AssertService(AssertHandler handler);

		AssertService& operator=(const AssertService& other);

		void Assert(const AssertRecord& record) const;

	private:
		AssertHandler mHandler;
		mutable std::atomic_int32_t mAssertDepth = 0;
	};
}

