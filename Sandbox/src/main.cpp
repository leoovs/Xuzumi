#include "Xuzumi/Instrumentation/InstrumentationProfile.hpp"
#include "Xuzumi/Instrumentation/InstrumentationSession.hpp"
#include "Xuzumi/Instrumentation/LoggerMacros.hpp"
#include "Xuzumi/Instrumentation/AssertMacros.hpp"

int main()
{
	Xuzumi::InstrumentationSession::Init<Xuzumi::DefaultInstrumentationProfile>();
	{
		XZ_LOG(XZ_APP_LOGGER, Info, "Hello from %s", "App");
		XZ_LOG(XZ_CORE_LOGGER, Info, "Hello from %s", "Runtime");

		XZ_ASSERT(XZ_APP_ASSERT, true, "Must be true");
		XZ_ASSERT(XZ_CORE_ASSERT, false, "Runtime error");
	}
	Xuzumi::InstrumentationSession::Shutdown();
}

