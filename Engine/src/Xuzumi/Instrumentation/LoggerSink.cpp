#include "Xuzumi/Instrumentation/LoggerSink.hpp"

namespace Xuzumi
{
	LoggerSink::LoggerSink(const LoggerSink& other)
		: mEmitter(other.mEmitter->Clone())
		, mFormatter(other.mFormatter->Clone())
	{}

	LoggerSink::LoggerSink(
		std::unique_ptr<LogEmitter> emitter,
		std::unique_ptr<LogFormatter> formatter
	)
		: mEmitter(std::move(emitter))
		, mFormatter(std::move(formatter))
	{}

	LoggerSink& LoggerSink::operator=(const LoggerSink& other)
	{
		if (this == &other)
		{
			return *this;
		}

		mEmitter = other.mEmitter->Clone();
		mFormatter = other.mFormatter->Clone();

		return *this;
	}


	void LoggerSink::Log(const LogRecord& record) const
	{
		if (nullptr == mEmitter || nullptr == mFormatter)
		{
			return;
		}

		mEmitter->Emit(mFormatter->FormatRecord(record));
	}
}

