#pragma once

#include "Xuzumi/Instrumentation/Logger.hpp"
#include "Xuzumi/Instrumentation/LogRecordBuilder.hpp"
#include "Xuzumi/Instrumentation/InstrumentationSession.hpp"

#define XZ_CORE_LOGGER \
	(::Xuzumi::InstrumentationSession::Get().GetCoreLogger())
#define XZ_APP_LOGGER \
	(::Xuzumi::InstrumentationSession::Get().GetAppLogger())
#define XZ_LOG(logger, level, format, ...) \
	(Xuzumi::Internal::LogImpl((logger), XZ_INTERNAL_MAKE_LOG_RECORD(level, format, ##__VA_ARGS__)))

namespace Xuzumi::Internal
{
	inline void LogImpl(const Logger& logger, const LogRecord& record)
	{
		logger.Log(record);
	}
}

