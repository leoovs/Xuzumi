#include "Xuzumi/Instrumentation/InstrumentationSession.hpp"

namespace Xuzumi
{
	void InstrumentationSession::Shutdown()
	{
		sInstance.reset();
	}

	InstrumentationSession& InstrumentationSession::Get()
	{
		if (sInstance)
		{
			// TODO: call `Init` with default profile and emit a warning.
		}

		return *sInstance;
	}

	const Logger& InstrumentationSession::GetAppLogger() const
	{
		return mAppLogger;
	}

	const Logger& InstrumentationSession::GetCoreLogger() const
	{
		return mCoreLogger;
	}

	const AssertService& InstrumentationSession::GetAppAssertService() const
	{
		return mAppAssertService;
	}

	const AssertService& InstrumentationSession::GetCoreAssertService() const
	{
		return mCoreAssertService;
	}

	void InstrumentationSession::Init(
		std::unique_ptr<InstrumentationProfile> profile
	)
	{
		sInstance.reset(new InstrumentationSession(std::move(profile)));
		sInstance->QueryProfile();
	}

	std::unique_ptr<InstrumentationSession> InstrumentationSession::sInstance;

	InstrumentationSession::InstrumentationSession(
		std::unique_ptr<InstrumentationProfile> profile
	)
		: mProfile(std::move(profile))
	{}

	void InstrumentationSession::QueryProfile()
	{
		mCoreLogger = mProfile->CreateCoreLogger();
		mAppLogger = mProfile->CreateAppLogger();
		mCoreAssertService = mProfile->CreateCoreAssertHandler();
		mAppAssertService = mProfile->CreateAppAssertHandler();
	}
}

