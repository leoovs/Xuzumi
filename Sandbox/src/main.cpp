#include <Xuzumi/Xuzumi.hpp>

#include "App.hpp"

int main()
{
	Xuzumi::InstrumentationSession::Init<Xuzumi::DefaultInstrumentationProfile>();
	{
		App().Run();
	}
	Xuzumi::InstrumentationSession::Shutdown();
}

