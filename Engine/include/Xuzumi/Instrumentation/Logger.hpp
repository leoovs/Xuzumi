#pragma once

#include "Xuzumi/Instrumentation/LoggerSink.hpp"

namespace Xuzumi
{
	class Logger
	{
	public:
		Logger() = default;
		explicit Logger(std::vector<LoggerSink> sinks);

		void Log(const LogRecord& record) const;

	private:
		std::vector<LoggerSink> mSinks;
	};
}

