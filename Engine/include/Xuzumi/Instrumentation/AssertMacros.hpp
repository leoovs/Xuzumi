#pragma once

#include "Xuzumi/Instrumentation/AssertService.hpp"
#include "Xuzumi/Instrumentation/AssertRecordBuilder.hpp"
#include "Xuzumi/Instrumentation/InstrumentationSession.hpp"

#define XZ_APP_ASSERT \
	(Xuzumi::InstrumentationSession::Get().GetAppAssertService())
#define XZ_CORE_ASSERT \
	(Xuzumi::InstrumentationSession::Get().GetCoreAssertService())
#define XZ_ASSERT(service, condition, ...) \
	(static_cast<bool>(condition) || \
	::Xuzumi::Internal::AssertImpl((service), XZ_INTERNAL_MAKE_ASSERT_RECORD(condition, ##__VA_ARGS__))) \

namespace Xuzumi::Internal
{
	inline bool AssertImpl(const AssertService& service, const AssertRecord& record)
	{
		service.Assert(record);
		return static_cast<bool>(record.LineNumber);
	}
}

