#pragma once

#include "Xuzumi/Instrumentation/LoggerSink.hpp"

namespace Xuzumi
{
	class LoggerSinkBuilder
	{
	public:
		template<typename EmitterT, typename... ArgsT>
		std::enable_if_t<
			std::is_base_of_v<LogEmitter, EmitterT>,
			LoggerSinkBuilder&
		>
		WithEmitter(ArgsT&&... args)
		{
			mSinkEmitter = std::make_unique<EmitterT>(
				std::forward<ArgsT>(args)...
			);
			return *this;
		}

		template<typename FormatterT, typename... ArgsT>
		std::enable_if_t<
			std::is_base_of_v<LogFormatter, FormatterT>,
			LoggerSinkBuilder&
		>
		WithFormatter(ArgsT&&... args)
		{
			mSinkFormatter = std::make_unique<FormatterT>(
				std::forward<ArgsT>(args)...
			);
			return *this;
		}

		LoggerSink Finish();

	private:
		std::unique_ptr<LogEmitter> mSinkEmitter;
		std::unique_ptr<LogFormatter> mSinkFormatter;
	};
}

