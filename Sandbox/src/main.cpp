#include "Xuzumi/Instrumentation/InstrumentationProfile.hpp"
#include "Xuzumi/Instrumentation/InstrumentationSession.hpp"
#include "Xuzumi/Instrumentation/LogFormatter.hpp"
#include "Xuzumi/Instrumentation/LoggerSinkBuilder.hpp"
#include "Xuzumi/Instrumentation/LoggerMacros.hpp"
#include "Xuzumi/Instrumentation/AssertMacros.hpp"

class SandboxInstrumProfile : public Xuzumi::InstrumentationProfile
{
public:
	Xuzumi::Logger CreateCoreLogger() const override
	{
		return Xuzumi::Logger({
			Xuzumi::LoggerSinkBuilder()
				.WithEmitter<Xuzumi::ConsoleLogEmitter>()
				.WithFormatter<Xuzumi::MinimalLogFormatter>()
				.Finish(),
			Xuzumi::LoggerSinkBuilder()
				.WithEmitter<Xuzumi::FileLogEmitter>("Xuzumi.Core.log")
				.WithFormatter<Xuzumi::VerboseLogFormatter>()
				.Finish()
		});
	}

	Xuzumi::Logger CreateAppLogger() const override
	{
		return Xuzumi::Logger({
			Xuzumi::LoggerSinkBuilder()
				.WithEmitter<Xuzumi::FileLogEmitter>("Xuzumi.App.log")
				.WithFormatter<Xuzumi::MinimalLogFormatter>()
				.Finish()
		});
	}

	Xuzumi::AssertHandler CreateAppAssertHandler() const override
	{
		return Xuzumi::AssertService::CreateHandlerFromLogger(XZ_APP_LOGGER);
	}

	Xuzumi::AssertHandler CreateCoreAssertHandler() const override
	{
		return Xuzumi::AssertService::CreateHandlerFromLogger(XZ_CORE_LOGGER);
	}
};

int main()
{
	Xuzumi::InstrumentationSession::Init<SandboxInstrumProfile>();
	{
		XZ_LOG(XZ_APP_LOGGER, Error, "Application error");
		XZ_LOG(XZ_CORE_LOGGER, Error, "Runtime error");

		XZ_ASSERT(XZ_CORE_ASSERT, 2 == 5 - 1, "Invalid %s", "arithmetic");
		XZ_ASSERT(XZ_APP_ASSERT, true == false);
	}
	Xuzumi::InstrumentationSession::Shutdown();
}

