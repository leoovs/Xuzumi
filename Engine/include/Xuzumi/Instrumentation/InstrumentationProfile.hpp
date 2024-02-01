#pragma once

#include "Xuzumi/Instrumentation/Logger.hpp"
#include "Xuzumi/Instrumentation/AssertService.hpp"
#include "Xuzumi/Instrumentation/LoggerSinkBuilder.hpp"

namespace Xuzumi
{
	class InstrumentationProfile
	{
	public:
		virtual ~InstrumentationProfile() = default;

		virtual const Logger& GetCoreLogger() const = 0;
		virtual const Logger& GetAppLogger() const = 0;

		virtual const AssertService& GetCoreAssertService() const = 0;
		virtual const AssertService& GetAppAssertService() const = 0;
	};

	class DefaultInstrumentationProfile final : public InstrumentationProfile
	{
	public:
		const Logger& GetCoreLogger() const override;
		const Logger& GetAppLogger() const override;

		const AssertService& GetCoreAssertService() const override;
		const AssertService& GetAppAssertService() const override;

	private:
		Xuzumi::Logger mCoreLogger{{
			LoggerSinkBuilder()
				.WithEmitter<ConsoleLogEmitter>()
				.WithFormatter<MinimalLogFormatter>()
				.Finish(),
			LoggerSinkBuilder()
				.WithEmitter<FileLogEmitter>("Xuzumi.Core.log")
				.WithFormatter<VerboseLogFormatter>()
				.Finish()
		}};

		Xuzumi::Logger mAppLogger{{
			LoggerSinkBuilder()
				.WithEmitter<ConsoleLogEmitter>()
				.WithFormatter<MinimalLogFormatter>()
				.Finish(),
			LoggerSinkBuilder()
				.WithEmitter<FileLogEmitter>("Xuzumi.App.log")
				.WithFormatter<VerboseLogFormatter>()
				.Finish()
		}};

		Xuzumi::AssertService mCoreAssertService{
			Xuzumi::AssertService::CreateHandlerFromLogger(mCoreLogger)
		};

		Xuzumi::AssertService mAppAssertService{
			Xuzumi::AssertService::CreateHandlerFromLogger(mAppLogger)
		};
	};
}

