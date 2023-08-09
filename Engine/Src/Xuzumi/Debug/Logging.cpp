#include "Xuzumi/Debug/Logging.hpp"

namespace Xuzumi
{
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// 
	// Logger
	// 
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	void Logger::Log(const LogData& data)
	{
		if (!mFormatter)
		{
			return;
		}

		mFormatter->FormatLogData(data);
		for (std::unique_ptr<ILogWriter>& writer : mWriters)
		{
			writer->Write(data, *mFormatter);
		}
	}

	void Logger::SetFormatter(std::unique_ptr<ILogFormatter> formatter)
	{
		if (formatter)
		{
			mFormatter = std::move(formatter);
		}
	}

	void Logger::AddWriter(std::unique_ptr<ILogWriter> writer)
	{
		if (writer)
		{
			mWriters.push_back(std::move(writer));
		}
	}

	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// 
	// LoggerConfigurator
	// 
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	LoggerConfigurator::LoggerConfigurator(ObserverPtr<Logger> configuredLogger)
		: mConfiguredLogger(configuredLogger)
	{
	}
}
