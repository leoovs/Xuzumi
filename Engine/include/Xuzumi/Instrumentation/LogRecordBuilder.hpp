#pragma once

#include "Xuzumi/Core/FuncName.hpp"
#include "Xuzumi/Instrumentation/LogRecord.hpp"

#define XZ_INTERNAL_MAKE_LOG_RECORD(level, format, ...) \
	(::Xuzumi::LogRecordBuilder() \
		.Level(::Xuzumi::LogLevel::level) \
		.AtFile(__FILE__) \
		.AtFunc(XZ_FUNCNAME) \
		.AtLine(__LINE__) \
		.Now() \
		.Format(format, ##__VA_ARGS__) \
		.Finish())

namespace Xuzumi
{
	class LogRecordBuilder
	{
	public:
		LogRecordBuilder& Level(LogLevel level);
		LogRecordBuilder& AtFile(std::string_view fileName);
		LogRecordBuilder& AtFunc(std::string_view scopeName);
		LogRecordBuilder& AtLine(std::int32_t lineNumber);
		LogRecordBuilder& Now();
		LogRecordBuilder& Format(std::string_view format, ...);

		LogRecord Finish();

	private:
		LogRecord mProduct;
	};
}

