#pragma once

#include "Xuzumi/Precompiled.hpp"
#include "Xuzumi/Instrumentation/LogEmitter.hpp"
#include "Xuzumi/Instrumentation/LogFormatter.hpp"

namespace Xuzumi
{
	class LoggerSink
	{
	public:
		LoggerSink() = default;

		LoggerSink(const LoggerSink& other);
		LoggerSink(LoggerSink&& other) = default;

		LoggerSink(
			std::unique_ptr<LogEmitter> emitter,
			std::unique_ptr<LogFormatter> formatter
		);

		LoggerSink& operator=(const LoggerSink& other);
		LoggerSink& operator=(LoggerSink&& other) = default;

		void Log(const LogRecord& record) const;

	private:
		std::unique_ptr<LogFormatter> mFormatter;
		std::unique_ptr<LogEmitter> mEmitter;
	};
}

