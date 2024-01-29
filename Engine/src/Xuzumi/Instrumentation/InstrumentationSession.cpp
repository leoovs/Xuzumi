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
				// TODO: init with default profile and report error.
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
			, mAppLogger(mProfile->CreateAppLogger())
			, mCoreLogger(mProfile->CreateCoreLogger())
		{}
}

