#pragma once

#include "Xuzumi/Instrumentation/LoggerSink.hpp"
#include "Xuzumi/Instrumentation/LogRecordBuilder.hpp"

namespace Xuzumi
{
	class Logger
	{
	public:
		Logger() = default;
		Logger(std::vector<LoggerSink> sinks);

		void Log(const LogRecord& record) const;

	private:
		std::vector<LoggerSink> mSinks;
	};
}

