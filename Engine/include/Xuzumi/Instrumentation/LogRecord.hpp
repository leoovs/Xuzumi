#pragma once

#include "Xuzumi/Precompiled.hpp"

namespace Xuzumi
{
	enum class LogLevel
	{
		Trace,
		Info,
		Warn,
		Error,
	};

	struct LogRecord
	{
		LogLevel Level = LogLevel::Trace;
		std::string FileName;
		std::int32_t LineNumber = 0;
		std::string FuncName;
		std::chrono::system_clock::time_point Timestamp;
		std::chrono::high_resolution_clock::time_point HighResTimestamp;
		std::string Message;
	};
}

