#include "Xuzumi/Instrumentation/InstrumentationSession.hpp"

#include "Xuzumi/Instrumentation/InstrumentationProfile.hpp"
#include "Xuzumi/Instrumentation/LoggerMacros.hpp"

namespace Xuzumi
{
	void InstrumentationSession::Shutdown()
	{
		sInstance.reset();
	}

	InstrumentationSession& InstrumentationSession::Get()
	{
		if (nullptr == sInstance)
		{
			Init<DefaultInstrumentationProfile>();
			XZ_LOG(
				XZ_CORE_LOGGER,
				Warn,
				"Trying to access uninitialized `InstrumentationSession`."
				" The runtime have fallen back to the `DefaultInstrumentationProfile`"
			);
		}

		return *sInstance;
	}

	const Logger& InstrumentationSession::GetCoreLogger() const
	{
		return mProfile->GetCoreLogger();
	}

	const Logger& InstrumentationSession::GetAppLogger() const
	{
		return mProfile->GetAppLogger();
	}

	const AssertService& InstrumentationSession::GetCoreAssertService() const
	{
		return mProfile->GetCoreAssertService();
	}

	const AssertService& InstrumentationSession::GetAppAssertService() const
	{
		return mProfile->GetAppAssertService();
	}

	void InstrumentationSession::Init(
		std::unique_ptr<InstrumentationProfile> profile
	)
	{
		sInstance.reset(new InstrumentationSession(std::move(profile)));
	}

	std::unique_ptr<InstrumentationSession> InstrumentationSession::sInstance;

	InstrumentationSession::InstrumentationSession(
		std::unique_ptr<InstrumentationProfile> profile
	)
		: mProfile(std::move(profile))
	{}
}

