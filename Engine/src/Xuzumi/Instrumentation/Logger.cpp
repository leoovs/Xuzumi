#include "Xuzumi/Instrumentation/Logger.hpp"

namespace Xuzumi
{
	Logger::Logger(std::vector<LoggerSink> sinks)
		: mSinks(std::move(sinks))
	{}

	void Logger::Log(const LogRecord& record) const
	{
		for (const LoggerSink& sink : mSinks)
		{
			sink.Log(record);
		}
	}
}

