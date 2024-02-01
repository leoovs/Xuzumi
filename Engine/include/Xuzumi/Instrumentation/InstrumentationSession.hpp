#pragma once

#include "Xuzumi/Instrumentation/AssertService.hpp"
#include "Xuzumi/Instrumentation/InstrumentationProfile.hpp"

namespace Xuzumi
{
	class InstrumentationSession
	{
	public:
		template<typename ProfileT, typename... ArgsT>
		static
		std::enable_if_t<std::is_base_of_v<InstrumentationProfile, ProfileT>, void>
		Init(ArgsT&&... args)
		{
			Init(std::make_unique<ProfileT>(std::forward<ArgsT>(args)...));
		}

		static void Shutdown();

		static InstrumentationSession& Get();

		~InstrumentationSession() = default;

		const Logger& GetCoreLogger() const;
		const Logger& GetAppLogger() const;

		const AssertService& GetCoreAssertService() const;
		const AssertService& GetAppAssertService() const;

	private:
		static void Init(std::unique_ptr<InstrumentationProfile> profile);

		InstrumentationSession(std::unique_ptr<InstrumentationProfile> profile);

		std::unique_ptr<InstrumentationProfile> mProfile;

		static std::unique_ptr<InstrumentationSession> sInstance;
	};
}

